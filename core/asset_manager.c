#include "asset_manager.h"

#include <stdarg.h>
#include <string.h>
#include "game.h"
#include "linked_list.h"
#include "texture.h"
#include "map.h"
#include "memory_alloc.h"
#include "errors.h"

// Args : asset_type_t asset_type, const char *asset_name
int create_asset(asset_t *asset, va_list args)
{
    asset->name = va_arg(args, const char *);
    asset->type = ASSET_NULL;
    asset->nb_refs = 0;

    return EXIT_SUCCESS;
}

int destroy_asset(asset_t *asset)
{
    switch(asset->type)
    {
      case ASSET_TEXTURE:
        destroy_texture(asset->data);
        break;

      case ASSET_MAP:
        if(destroy_map(asset->data)) return_failure_int;
        break;

      case ASSET_NULL:
        break;

      default:
        error_printf("Asset type not valid : %d", asset->type);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

// No args
int create_asset_header_def(asset_header_def_t *asset_header_def, va_list args __attribute__((unused)))
{
    asset_header_def->name = NULL;

    return EXIT_SUCCESS;
}

int destroy_asset_header_def(asset_header_def_t *asset_header_def)
{
    free(asset_header_def->name);

    return EXIT_SUCCESS;
}

int asset_manager_init(void)
{
    linked_list_init(&game.asset_manager.assets, sizeof(asset_t), (allocator_t)create_asset, (deleter_t)destroy_asset);
    linked_list_init(&game.asset_manager.assets_header_defs, sizeof(asset_header_def_t), (allocator_t)create_asset_header_def, (deleter_t)destroy_asset_header_def);

    game.asset_manager.asset_file = fopen(ASSET_FILE_NAME, "rb");
    if(!game.asset_manager.asset_file)
    {
        error_printf("Failed to open asset file : %d", errno);
        return EXIT_FAILURE;
    }

    uint32_t nb_assets;
    if(fread(&nb_assets, sizeof(nb_assets), 1, game.asset_manager.asset_file) != 1)
    {
        error_printf("Failed to read asset file : %d", ferror(game.asset_manager.asset_file));
        return EXIT_FAILURE;
    }

    if(linked_list_reserve(&game.asset_manager.assets_header_defs, nb_assets)) return_failure_int;

    for(size_t i = 0; i < nb_assets; i++)
    {
        elem_t *asset_header_def_elem = create_elem(&game.asset_manager.assets_header_defs);
        if(!asset_header_def_elem) return_failure_int;
        asset_header_def_t *asset_header_def = asset_header_def_elem->data;

        {
            size_t name_size = 0;
            int ch = EOF;

            do
            {
                name_size++;

                asset_header_def->name = realloc(asset_header_def->name, name_size);

                ch = fgetc(game.asset_manager.asset_file);
                if(ch == EOF)
                {
                    error_printf("Failed to read asset file : %d", ferror(game.asset_manager.asset_file));
                    return EXIT_FAILURE;
                }

                asset_header_def->name[name_size - 1] = ch;
            }
            while(ch != '\0');

            asset_header_def->name[name_size] = '\0';
        }

        if(fread(&asset_header_def->start, sizeof(asset_header_def->start), 1, game.asset_manager.asset_file) != 1)
        {
            error_printf("Failed to read asset file : %d", ferror(game.asset_manager.asset_file));
            return EXIT_FAILURE;
        }

        linked_list_push_back(&game.asset_manager.assets_header_defs, asset_header_def_elem);
    }

    debug_printf("Initialized asset manager.");
    return EXIT_SUCCESS;
}

int asset_manager_exit(void)
{
    if(linked_list_exit(&game.asset_manager.assets_header_defs)) return_failure_int;
    if(linked_list_exit(&game.asset_manager.assets)) return_failure_int;

    if(game.asset_manager.asset_file)
    {
        if(fclose(game.asset_manager.asset_file))
        {
            error_printf("Failed to close asset file : %d", errno);
            return EXIT_FAILURE;
        }
    }

    debug_printf("Exited asset manager.");
    return EXIT_SUCCESS;
}

// Only for this file
// No args
static inline bool condition_is_asset_unused(elem_t *elem, void *arg __attribute__((unused)))
{
    return !((asset_t *)elem->data)->nb_refs;
}

inline int asset_manager_collect_garbage(void)
{
    if(linked_list_remove_if(&game.asset_manager.assets, condition_is_asset_unused, NULL)) return_failure_int;

    return EXIT_SUCCESS;
}

// Only for this file
// Arg : const char *asset_name
static inline bool condition_is_asset_name(elem_t *elem, void *arg)
{
    return !strcmp(((asset_t *)elem->data)->name, (const char *)arg);
}

//Only for this file
// Arg : const char *asset_name
static inline bool condition_is_asset_header_def_name(elem_t *elem, void *arg)
{
    return !strcmp(((asset_header_def_t *)elem->data)->name, (const char *)arg);
}

asset_t *asset_manager_load_asset(char *asset_name, asset_type_t asset_type)
{
    // Check if asset has alread been loaded.
    elem_t *asset_elem = linked_list_get_if(&game.asset_manager.assets, condition_is_asset_name, (void *)asset_name);
    if(asset_elem)
    {
        asset_t *asset = asset_elem->data;
        asset->nb_refs++;

        debug_printf("Asset \"%s\" has already been loaded, nb refs : %d", asset->name, asset->nb_refs);
    }
    else
    {
        // If it didn't, load it
        elem_t *asset_header_def_elem = linked_list_get_if(&game.asset_manager.assets_header_defs, condition_is_asset_header_def_name, (void *)asset_name);
        if(!asset_header_def_elem)
        {
            error_printf("Asset doesn't exist in asset file : %s", asset_name);
            return NULL;
        }
        asset_header_def_t *asset_header_def = asset_header_def_elem->data;

        asset_elem = create_elem(&game.asset_manager.assets, asset_name);
        if(!asset_elem) return_failure_ptr;
        asset_t *asset = asset_elem->data;

        if(fseek(game.asset_manager.asset_file, asset_header_def->start, SEEK_SET))
        {
            error_printf("Failed to seek in asset file : %d", errno);
            return NULL;
        }

        switch(asset_type)
        {
          case ASSET_TEXTURE:
            asset->data = load_texture();
            break;

          case ASSET_MAP:
            asset->data = load_map();
            break;

          default:
            error_printf("Asset type doesn't exists : %d", asset_type);
            break;
        }

        if(!asset->data)
        {
            destroy_elem(&game.asset_manager.assets, asset_elem); // returns failure anyway
            return_failure_ptr;
        }

        asset->type = asset_type;
        asset->nb_refs++;

        linked_list_push_back(&game.asset_manager.assets, asset_elem);

        debug_printf("Asset \"%s\" has been loaded.", asset->name);
    }

    return asset_elem->data;
}

int asset_manager_let_go_asset(char *asset_name)
{
    elem_t *asset_elem = linked_list_get_if(&game.asset_manager.assets, condition_is_asset_name, (void *)asset_name);
    if(!asset_elem)
    {
        error_printf("Asset doesn't exists : %s", asset_name);
        return EXIT_FAILURE;
    }

    asset_t *asset = asset_elem->data;
    asset->nb_refs--;

    debug_printf("Asset \"%s\" has been let go, nb refs : %d", asset->name, asset->nb_refs);

    return EXIT_SUCCESS;
}

asset_t *asset_manager_get_asset(char *asset_name)
{
    elem_t *asset_elem = linked_list_get_if(&game.asset_manager.assets, condition_is_asset_name, (void *)asset_name);
    if(!asset_elem)
    {
        error_printf("Asset \"%s\" hasn't been loaded yet.", asset_name);
        return NULL;
    }

    return asset_elem->data;
}
