#include "game.h"

#include <stdlib.h>
#include "update_time.h"
#include "errors.h"

game_t game;

int main(void)
{
    if(game_init()) goto_failure;

    while(game.is_running)
    {
        update_time();
        game_handle_event();
        if(game_update()) goto_failure;
        if(game_render()) goto_failure;
    }

    if(game_exit()) return_failure_int;

    return EXIT_SUCCESS;

failure:
    game_exit(); // return failure anyway

    return EXIT_FAILURE;
}
