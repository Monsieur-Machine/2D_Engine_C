#include <time.h>
#include <event.h>
#include <display.h>
#include "game.h"
#include "components.h"

void game_init(void)
{
    texture_manager_init(&game.texture_manager);
    texture_manager_load_builtin_textures(&game.texture_manager);

    entity_manager_init(&game.entity_manager);

    entity_t *player = create_entity(0);

    entity_manager_add_entity(&game.entity_manager, player);

    rect_t player_bounds = {10.0f, 10.0f, 32.0f, 32.0f};

    add_component(player, create_component(TRANSFORM_COMPONENT), player_bounds, PLAYER_DEFAULT_SPEED);
    add_component(player, create_component(SPRITE_COMPONENT), "player_idle_sheet");
    add_component(player, create_component(ANIMATION_SYSTEM), 4, 0, PLAYER_DEFAULT_IDLE_ANIMATION_SPEED, true, true, false);
    add_component(player, create_component(PLAYER_SYSTEM));

    game.is_running = true;
}

void game_handle_event(void)
{
    event_t event;

    while(pollevent(&event))
    {
        if(event.type == KEYEV_DOWN)
            game.keys[event.key.keysym.scancode] = true;
        else if(event.type == KEYEV_UP)
            game.keys[event.key.keysym.scancode] = false;

        if(event.type == SDL_QUIT)
            game.is_running = false;
    }
}

static inline void update_time(void)
{
    clock_t last_clock_state = clock();
    clock_t start_clock = clock();
    game.delta_time_ms = (double)(start_clock - last_clock_state) * 1000.0f / CLOCKS_PER_SEC;
    last_clock_state = start_clock;
}

void game_update(void)
{
    update_time();

    entity_manager_update(&game.entity_manager);
}

void game_render(void)
{
    dclear();

    entity_manager_draw(&game.entity_manager);

    dupdate();
}

void game_exit(void)
{
    entity_manager_clear(&game.entity_manager);
    texture_manager_clear(&game.texture_manager);
}
