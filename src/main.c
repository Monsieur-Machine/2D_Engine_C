#include "game.h"

//#define TEXTURE_TEST
//#define LINKED_LIST_TEST

game_t game;

int main(void)
{
    game_init();

    while(game.is_running)
    {
        game_handle_event();
        game_update();
        game_render();
    }

    game_exit();

    return 0;
}


// Texture Unit Tests
#ifdef TEXTURE_TEST

#include "texture_manager.h"

int main(void)
{
    dclear(C_WHITE);

    texture_manager_t texture_manager;
    texture_manager_init(&texture_manager);
    texture_manager_load_builtin_textures(&texture_manager);

    texture_t *texture = texture_manager_get_texture(&texture_manager, "player_run_sheet");

    rect_t srcR = {0,0,32,32};
    rect_t dstR = srcR;

    draw_texture(texture, &src, &dst);

    src.x = 33;
    dst.x = 33;

    draw_texture(texture, &src, &dst);

    texture_manager_remove_texture(&texture_manager, "player_run_sheet");

    src.w = 128;
    dst.x = 0;
    dst.y = 33;

    texture_manager_draw_texture(&texture_manager, "player_idle_sheet", &src, &dst);

    texture_manager_clear(&texture_manager);

    dupdate();
    getkey();

    return 1;
}

#endif // TEXTURE_TEST

// Linked List Unit Tests
#ifdef LINKED_LIST_TEST

#include "linked_list.h"

int main(void)
{
    dclear(C_WHITE);

    linked_list_t linked_list;
    linked_list_init(&linked_list, sizeof(int), NULL);
    if(linked_list.first == linked_list.last &&
       linked_list.first == NULL)
    {
        dtext(1, 0, C_BLACK, "linked_list is initialised correctly");
    }
    else
    {
        goto Exit;
    }

    elem_t *elem1 = NULL;
    elem_t *elem2 = NULL;
    elem_t *elem3 = NULL;
    elem_t *elem4 = NULL;

    elem1 = elem_create(&linked_list);
    if(elem1 && elem1->data)
    {
        dtext(1, 10, C_BLACK, "elem1 is initalised correctly");
    }
    else
    {
        goto Exit;
    }

    linked_list_push_back_elem(&linked_list, elem1);
    if(linked_list.first == linked_list.last &&
       linked_list.first == elem1 &&
       !linked_list.first->prev &&
       !linked_list.first->next &&
       !linked_list.last->prev &&
       !linked_list.last->next)
    {
        dtext(1, 20, C_BLACK, "elem1 is correctly pushed back");
    }
    else
    {
        goto Exit;
    }

    linked_list_pop_back(&linked_list);
    if(linked_list.first == linked_list.last &&
       linked_list.first == NULL)
    {
        dtext(1, 30, C_BLACK, "elem1 is correctly poped back");
    }
    else
    {
        goto Exit;
    }

    elem1 = elem_create(&linked_list);
    elem2 = elem_create(&linked_list);
    elem3 = elem_create(&linked_list);
    elem4 = elem_create(&linked_list);

    linked_list_push_back_elem(&linked_list, elem1);
    linked_list_push_back_elem(&linked_list, elem2);
    if(linked_list.first == elem1 &&
       linked_list.last == elem2 &&
       !linked_list.first->prev &&
       linked_list.first->next &&
       linked_list.last->prev &&
       !linked_list.last->next &&
       linked_list.first->next == elem2 &&
       linked_list.last->prev == elem1)
    {
        dtext(1, 40, C_BLACK, "elem1 and elem2 are correctly pushed back");
    }
    else
    {
        goto Exit;
    }

    linked_list_push_front_elem(&linked_list, elem3);
    linked_list_push_front_elem(&linked_list, elem4);
    if(!linked_list.first->prev &&
       !linked_list.last->next &&
       linked_list.first == elem4 &&
       linked_list.first->next == elem3 &&
       linked_list.first->next->next == elem1 &&
       linked_list.first->next->next->next == elem2 &&
       linked_list.last == elem2 &&
       linked_list.last->prev == elem1 &&
       linked_list.last->prev->prev == elem3 &&
       linked_list.last->prev->prev->prev == elem4)
    {
        dtext(1, 50, C_BLACK, "elem3 and elem4 are correctly pushed front");
    }
    else
    {
        goto Exit;
    }

    if(linked_list_get_elem(&linked_list, 2) == elem1)
    {
        dtext(1, 60, C_BLACK, "elem4 is got correctly");
    }
    else
    {
        goto Exit;
    }

    linked_list_pop_back(&linked_list);
    if(!linked_list.first->prev &&
       !linked_list.last->next &&
       linked_list.first == elem4 &&
       linked_list.first->next == elem3 &&
       linked_list.first->next->next == elem1 &&
       linked_list.last == elem1 &&
       linked_list.last->prev == elem3 &&
       linked_list.last->prev->prev == elem4)
    {
        dtext(1, 70, C_BLACK, "elem2 is correctly poped back");
    }
    else
    {
        goto Exit;
    }

    linked_list_remove(&linked_list, 1);
    if(!linked_list.first->prev &&
       !linked_list.last->next &&
       linked_list.first == elem4 &&
       linked_list.first->next == elem1 &&
       linked_list.last == elem1 &&
       linked_list.last->prev == elem4)
    {
        dtext(1, 80, C_BLACK, "elem3 is correctly removed");
    }
    else
    {
        goto Exit;
    }

    linked_list_clear(&linked_list);
    if(linked_list.first == linked_list.last &&
       linked_list.first == NULL)
    {
        dtext(1, 90, C_BLACK, "linked list is correctly cleared");
    }
    else
    {
        goto Exit;
    }

    elem1 = elem_create(&linked_list);
    elem2 = elem_create(&linked_list);
    elem3 = elem_create(&linked_list);
    elem4 = elem_create(&linked_list);

    linked_list_push_back_elem(&linked_list, elem1);
    linked_list_push_back_elem(&linked_list, elem2);
    linked_list_push_front_elem(&linked_list, elem3);
    if(linked_list.first == elem3 &&
       linked_list.last == elem2 &&
       !linked_list.first->prev &&
       linked_list.first->next &&
       linked_list.last->prev &&
       !linked_list.last->next &&
       linked_list.first->next == elem1 &&
       linked_list.first->next->next == elem2 &&
       linked_list.last->prev == elem1 &&
       linked_list.last->prev->prev == elem3)
    {
        dtext(1, 100, C_BLACK, "elem1, elem2 and elem3 are correctly pushed");
    }
    else
    {
        goto Exit;
    }

    linked_list_insert_elem(&linked_list, elem4, 1);
    if(linked_list.first == elem3 &&
       linked_list.last == elem2 &&
       !linked_list.first->prev &&
       linked_list.first->next &&
       linked_list.last->prev &&
       !linked_list.last->next &&
       linked_list.first->next == elem4 &&
       linked_list.first->next->next == elem1 &&
       linked_list.first->next->next->next == elem2 &&
       linked_list.last->prev == elem1 &&
       linked_list.last->prev->prev == elem4 &&
       linked_list.last->prev->prev->prev == elem3)
    {
        dtext(1, 110, C_BLACK, "elem3 is correctly inserted");
    }
    else
    {
        goto Exit;
    }

    linked_list_clear(&linked_list);

  Exit:
    dupdate();
    getkey();

    return 1;
}

#endif // LINKED_LIST_TEST
