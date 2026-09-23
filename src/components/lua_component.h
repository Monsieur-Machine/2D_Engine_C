#ifndef LUA_COMPONENT_H
#define LUA_COMPONENT_H

#include <lua.h>
#include "ecs.h"
#include "errors.h"

typedef struct lua_component_data_t {
    lua_State *lua_state;
} lua_component_data_t;

int lua_component_init(component_t *component);
int lua_component_update(component_t *component);
#if DEBUG >= 2
int lua_component_draw(const component_t *component);
#endif // DEBUG >= 2
int lua_component_destroy(component_t *component);

#endif // LUA_COMPONENT_H