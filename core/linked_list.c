#include <stdarg.h>
#include "memory_alloc.h"
#include "linked_list.h"

elem_t *elem_create(const linked_list_t *linked_list)
{
    elem_t *tmp;
    tmp = malloc(sizeof(elem_t));
    if(!tmp) return NULL;

    tmp->data = malloc(linked_list->data_size);
    if(!tmp->data)
    {
        free(tmp);
        return NULL;
    }

    return tmp;
}

inline void destroy_elem(elem_t *elem, const deleter_t elem_deleter)
{
    if(!elem) return;

    if(elem_deleter)
        elem_deleter(elem->data);
    else
        free(elem->data);

    free(elem);
}

void linked_list_init(linked_list_t *linked_list, const size_t data_size, const deleter_t elem_deleter)
{
    linked_list->first = NULL;
    linked_list->last = NULL;
    linked_list->size = 0;

    linked_list->data_size = data_size;
    linked_list->elem_deleter = elem_deleter;
}

void linked_list_push_back_elem(linked_list_t *linked_list, elem_t *elem)
{
    if(!elem) return;

    elem->prev = linked_list->last;
    elem->next = NULL;

    if(linked_list->last)
        linked_list->last->next = elem;
    else
        linked_list->first = elem;

    linked_list->last = elem;

    linked_list->size++;
}

void linked_list_push_front_elem(linked_list_t *linked_list, elem_t *elem)
{
    if(!elem) return;

    elem->next = linked_list->first;
    elem->prev = NULL;

    if(linked_list->first)
        linked_list->first->prev = elem;
    else
        linked_list->last = elem;

    linked_list->first = elem;

    linked_list->size++;
}

void linked_list_push_back(linked_list_t *linked_list, void *data)
{
    elem_t *tmp = malloc(sizeof(elem_t));
    if(!tmp) return;
    tmp->data = data;

    linked_list_push_back_elem(linked_list, tmp);
}

void linked_list_push_front(linked_list_t *linked_list, void *data)
{
    elem_t *tmp = malloc(sizeof(elem_t));
    if(!tmp) return;
    tmp->data = data;

    linked_list_push_front_elem(linked_list, tmp);
}

void linked_list_pop_back(linked_list_t *linked_list)
{
    elem_t *tmp = linked_list->last;
    if(!tmp) return;

    linked_list->last = tmp->prev;

    if(linked_list->last)
        linked_list->last->next = NULL;
    else
        linked_list->first = NULL;

    destroy_elem(tmp, linked_list->elem_deleter);

    linked_list->size--;
}

void linked_list_pop_front(linked_list_t *linked_list)
{
    elem_t *tmp = linked_list->first;
    if(!tmp) return;

    linked_list->first = tmp->next;

    if(linked_list->first)
        linked_list->first->prev = NULL;
    else
        linked_list->last = NULL;

    destroy_elem(tmp, linked_list->elem_deleter);

    linked_list->size--;
}

void linked_list_clear(linked_list_t *linked_list)
{
    elem_t *actual_elem = linked_list->first;
    elem_t *tmp;

    while(actual_elem)
    {
        tmp = actual_elem;

        actual_elem = actual_elem->next;

        destroy_elem(tmp, linked_list->elem_deleter);
    }

    linked_list->first = NULL;
    linked_list->last = NULL;

    linked_list->size = 0;
}

inline bool linked_list_is_empty(const linked_list_t *linked_list)
{
   return !linked_list->first;
}

inline bool linked_list_is_in_bound(const linked_list_t *linked_list, size_t index)
{
    return index < linked_list->size;
}

elem_t *linked_list_get_elem(const linked_list_t *linked_list, size_t index)
{
    if(!linked_list_is_in_bound(linked_list, index)) return NULL;

    if(index == 0)
        return linked_list->first;
    if(index == linked_list->size - 1)
        return linked_list->last;

    elem_t *actual_elem = linked_list->first;
    if(!actual_elem) return NULL;

    for (size_t i = 0; i < index; i++)
    {
        actual_elem = actual_elem->next;
    }

    return actual_elem;
}

void *linked_list_get(const linked_list_t *linked_list, size_t index)
{
    elem_t *tmp = linked_list_get_elem(linked_list, index);
    if(!tmp) return NULL;
    return tmp->data;
}

void linked_list_insert_elem(linked_list_t *linked_list, elem_t *elem, size_t index)
{
    if(!elem) return;
    if(!linked_list_is_in_bound(linked_list, index)) return;

    if(index == 0)
    {
        linked_list_push_front_elem(linked_list, elem);
    }
    else if(index == (linked_list->size - 1))
    {
        linked_list_push_back_elem(linked_list, elem);
    }
    else
    {
        elem_t *next_insert_elem = linked_list_get_elem(linked_list, index);
        elem_t *prev_insert_elem = next_insert_elem->prev;

        elem->prev = prev_insert_elem;
        elem->next = next_insert_elem;

        prev_insert_elem->next = elem;
        next_insert_elem->prev = elem;

        linked_list->size++;
    }
}

void linked_list_insert(linked_list_t *linked_list, void *data, size_t index)
{
    elem_t *tmp = malloc(sizeof(elem_t));
    if(!tmp) return;
    tmp->data = data;

    linked_list_insert_elem(linked_list, tmp, index);
}

void linked_list_remove_elem(linked_list_t *linked_list, elem_t *elem)
{
    if(!elem) return;

    if(elem == linked_list->first)
    {
        linked_list_pop_front(linked_list);
    }
    else if(elem == linked_list->last)
    {
        linked_list_pop_back(linked_list);
    }
    else
    {
        elem_t *prev_insert_elem = elem->prev;
        elem_t *next_insert_elem = elem->next;

        prev_insert_elem->next = next_insert_elem;
        next_insert_elem->prev = prev_insert_elem;

        destroy_elem(elem, linked_list->elem_deleter);

        linked_list->size--;
    }
}

void linked_list_remove(linked_list_t *linked_list, size_t index)
{
    elem_t *tmp = linked_list_get_elem(linked_list, index);
    if(tmp) linked_list_remove_elem(linked_list, tmp);
}

elem_t *linked_list_get_elem_if(const linked_list_t *linked_list, const condition_t condition, ...)
{
    if(!condition) return NULL;

    va_list args;
    va_start(args, condition);

    elem_t *actual_elem = linked_list->first;
    elem_t *next_elem;

    while(actual_elem)
    {
        next_elem = actual_elem->next;

        va_list args_copy;
        va_copy(args_copy, args);

        if(condition(actual_elem, args_copy))
        {
            va_end(args_copy);
            va_end(args);

            return actual_elem;
        }

        va_end(args_copy);

        actual_elem = next_elem;
    }

    va_end(args);

    return NULL;
}

void *linked_list_get_if(const linked_list_t *linked_list, const condition_t condition, ...)
{
    if(!condition) return NULL;

    va_list args;
    va_start(args, condition);

    elem_t *actual_elem = linked_list->first;
    elem_t *next_elem;

    while(actual_elem)
    {
        next_elem = actual_elem->next;

        va_list args_copy;
        va_copy(args_copy, args);

        if(condition(actual_elem, args_copy))
        {
            va_end(args_copy);
            va_end(args);

            return actual_elem->data;
        }

        va_end(args_copy);

        actual_elem = next_elem;
    }

    va_end(args);

    return NULL;
}

void linked_list_remove_if(linked_list_t *linked_list, const condition_t condition, ...)
{
    if(!condition) return;

    va_list args;
    va_start(args, condition);

    elem_t *actual_elem = linked_list->first;
    elem_t *next_elem;

    while(actual_elem)
    {
        next_elem = actual_elem->next;

        va_list args_copy;
        va_copy(args_copy, args);

        if(condition(actual_elem, args_copy))
        {
            linked_list_remove_elem(linked_list, actual_elem);
        }

        va_end(args_copy);

        actual_elem = next_elem;
    }

    va_end(args);
}

void linked_list_for_each(const linked_list_t *linked_list, const action_t action, ...)
{
    if(!action) return;

    va_list args;
    va_start(args, action);

    elem_t *actual_elem = linked_list->first;

    while(actual_elem)
    {
        va_list args_copy;
        va_copy(args_copy, args);

        action(actual_elem, args_copy);

        va_end(args_copy);

        actual_elem = actual_elem->next;
    }

    va_end(args);
}
