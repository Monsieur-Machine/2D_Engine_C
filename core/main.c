#include "game.h"

#include <SDL3/SDL.h>
#include <stdlib.h>
#include "errors.h"

game_t game;

static inline void update_time(void)
{
    static uint64_t last_clock_state = 0;
    uint64_t start_clock = SDL_GetTicksNS();
    game.delta_time_ms = SDL_NS_TO_MS((float)(start_clock - last_clock_state));
    if(game.delta_time_ms > 100.0f) game.delta_time_ms = 100.0f;
    last_clock_state = start_clock;
}

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
