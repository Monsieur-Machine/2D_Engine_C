#include "lua_component.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "game.h"
#include "ecs.h"
#include "memory_alloc.h"
#include "errors.h"

inline int lua_component_init(component_t *component)
{
    lua_component_data_t *component_data = component->data;

    component_data->lua_state = luaL_newstate();

    luaL_openlibs(component_data->lua_state);

    luaL_dostring(component_data->lua_state, "function f (a) print(a) end");

    return EXIT_SUCCESS;
}

inline int lua_component_update(component_t *component __attribute__((unused)))
{
    lua_component_data_t *component_data = component->data;

    luaL_dostring(component_data->lua_state, "f(\"prout\")");

    return EXIT_SUCCESS;
}

#if DEBUG >= 2
#include "display.h"

inline int lua_component_draw(const component_t *component)
{
    return EXIT_SUCCESS;
}
#endif // DEBUG >= 2

inline int lua_component_destroy(component_t *component)
{
    lua_component_data_t *component_data = component->data;

    lua_close(component_data->lua_state);

    free(component_data);

    return EXIT_SUCCESS;
}
