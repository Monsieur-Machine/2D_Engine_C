#include <stdarg.h>
#include "memory_alloc.h"
#include "linked_list.h"
#include "errors.h"

elem_t *create_elem(linked_list_t *linked_list, ...)
{
    assert("Linked list cannot be NULL" && linked_list);

    elem_t *elem;

    if(linked_list->reserved)
    {
        elem = linked_list->reserved;
        linked_list->reserved = elem->next;
    }
    else
    {
        elem = malloc(sizeof(elem_t));
        if(!elem)
        {
            error_printf("Failed to allocate memory : %d", errno);
            return NULL;
        }

        elem->data = malloc(linked_list->data_size);
        if(!elem->data)
        {
            error_printf("Failed to allocate memory : %d", errno);
            free(elem);
            return NULL;
        }
    }

    if(linked_list->allocator)
    {
        va_list args, args_copy;
        va_start(args, linked_list);
        va_copy(args_copy, args);

        if(linked_list->allocator(elem->data, args_copy)) return_failure_ptr;

        va_end(args_copy);
        va_end(args);
    }

    return elem;
}

inline int destroy_elem(linked_list_t *linked_list, elem_t *elem)
{
    assert("Linked list cannot be NULL" && linked_list);

    if(linked_list->deleter)
        if(linked_list->deleter(elem->data)) return_failure_int;

    if(linked_list->size < linked_list->target_max_size)
    {
        elem->next = linked_list->reserved;
        linked_list->reserved = elem;
    }
    else
    {
        free(elem->data);
        free(elem);
    }

    return EXIT_SUCCESS;
}

void linked_list_init(linked_list_t *linked_list, size_t data_size, const allocator_t allocator, const deleter_t elem_deleter)
{
    assert("Linked list cannot be NULL" && linked_list);

    linked_list->data_size = data_size;
    linked_list->allocator = allocator;
    linked_list->deleter = elem_deleter;

    linked_list->first = NULL;
    linked_list->last = NULL;
    linked_list->size = 0;

    linked_list->reserved = NULL;
    linked_list->target_max_size = 0;
}

int linked_list_exit(linked_list_t *linked_list)
{
    assert("Linked list cannot be NULL" && linked_list);

    linked_list->target_max_size = 0;

    elem_t *current_elem = linked_list->first;
    elem_t *tmp;

    while(current_elem)
    {
        tmp = current_elem;

        current_elem = current_elem->next;

        if(destroy_elem(linked_list, tmp)) return_failure_int;
    }

    current_elem = linked_list->reserved;

    while(current_elem)
    {
        tmp = current_elem;

        current_elem = current_elem->next;

        free(tmp->data);
        free(tmp);
    }

    return EXIT_SUCCESS;
}

int linked_list_clear(linked_list_t *linked_list)
{
    assert("Linked list cannot be NULL" && linked_list);

    elem_t *current_elem = linked_list->first;
    elem_t *tmp;

    while(current_elem)
    {
        tmp = current_elem;

        current_elem = current_elem->next;

        if(destroy_elem(linked_list, tmp)) return_failure_int;
    }

    linked_list->first = NULL;
    linked_list->last = NULL;

    linked_list->size = 0;

    return EXIT_SUCCESS;
}

int linked_list_reserve(linked_list_t *linked_list, size_t target_max_size)
{
    assert("Linked list cannot be NULL" && linked_list);

    linked_list->target_max_size = target_max_size;

    for(size_t i = linked_list->size; i < target_max_size; i++)
    {
        elem_t *elem = malloc(sizeof(elem_t));
        if(!elem)
        {
            error_printf("Failed to allocate memory : %d", errno);
            return EXIT_FAILURE;
        }

        elem->data = malloc(linked_list->data_size);
        if(!elem->data)
        {
            free(elem);
            error_printf("Failed to allocate memory : %d", errno);
            return EXIT_FAILURE;
        }

        elem->next = linked_list->reserved;
        linked_list->reserved = elem;
    }

    return EXIT_SUCCESS;
}

void linked_list_push_back(linked_list_t *linked_list, elem_t *elem)
{
    assert("Linked list cannot be NULL" && linked_list);

    elem->prev = linked_list->last;
    elem->next = NULL;

    if(linked_list->last)
        linked_list->last->next = elem;
    else
        linked_list->first = elem;

    linked_list->last = elem;

    linked_list->size++;
}

void linked_list_push_front(linked_list_t *linked_list, elem_t *elem)
{
    assert("Linked list cannot be NULL" && linked_list);

    elem->next = linked_list->first;
    elem->prev = NULL;

    if(linked_list->first)
        linked_list->first->prev = elem;
    else
        linked_list->last = elem;

    linked_list->first = elem;

    linked_list->size++;
}

elem_t *linked_list_pop_back(linked_list_t *linked_list)
{
    assert("Linked list cannot be NULL" && linked_list);

    elem_t *elem = linked_list->last;
    if(!elem)
    {
        error_printf("No elem to pop back.");
        return NULL;
    }

    linked_list->last = elem->prev;

    if(linked_list->last)
        linked_list->last->next = NULL;
    else
        linked_list->first = NULL;

    linked_list->size--;

    return elem;
}

elem_t *linked_list_pop_front(linked_list_t *linked_list)
{
    assert("Linked list cannot be NULL" && linked_list);

    elem_t *elem = linked_list->first;
    if(!elem)
    {
        error_printf("No elem to pop front.");
        return NULL;
    }

    linked_list->first = elem->next;

    if(linked_list->first)
        linked_list->first->prev = NULL;
    else
        linked_list->last = NULL;

    linked_list->size--;

    return elem;
}

elem_t *linked_list_get(const linked_list_t *linked_list, size_t index)
{
    assert("Linked list cannot be NULL" && linked_list);

    if(index >= linked_list->size)
    {
        error_printf("List index out of range.");
        return NULL;
    }

    if(index == 0)
        return linked_list->first;

    if(index == linked_list->size - 1)
        return linked_list->last;

    elem_t *current_elem = linked_list->first;
    if(!current_elem)
    {
        error_printf("Failed to get element : linked list is empty.");
        return NULL;
    }

    for (size_t i = 0; i < index; i++)
    {
        current_elem = current_elem->next;
    }

    return current_elem;
}

int linked_list_insert(linked_list_t *linked_list, elem_t *elem, size_t index) // XXX TODO SIMPLIFY INSERT FUNC XXX //
{
    assert("Linked list cannot be NULL" && linked_list);

    if(index == 0)
    {
        linked_list_push_front(linked_list, elem);
        return EXIT_SUCCESS;
    }
    else if(index == linked_list->size)
    {
        linked_list_push_back(linked_list, elem);
        return EXIT_SUCCESS;
    }

    elem_t *next_insert_elem = linked_list_get(linked_list, index);
    if(!next_insert_elem) return_failure_int;
    elem_t *prev_insert_elem = next_insert_elem->prev;

    elem->prev = prev_insert_elem;
    elem->next = next_insert_elem;

    prev_insert_elem->next = elem;
    next_insert_elem->prev = elem;

    linked_list->size++;

    return EXIT_SUCCESS;
}

int linked_list_remove(linked_list_t *linked_list, elem_t *elem)
{
    assert("Linked list cannot be NULL" && linked_list);

    elem_t *prev_elem = elem->prev;
    elem_t *next_elem = elem->next;

    if(prev_elem) prev_elem->next = next_elem;
    else          linked_list->first = next_elem;
    if(next_elem) next_elem->prev = prev_elem;
    else          linked_list->last = prev_elem;

    if(destroy_elem(linked_list, elem)) return_failure_int;

    linked_list->size--;

    return EXIT_SUCCESS;
}

elem_t *linked_list_get_if(const linked_list_t *linked_list, const condition_t condition, void *arg)
{
    assert("Linked list cannot be NULL" && linked_list);
    assert("Condition cannot be NULL" && condition);

    elem_t *current_elem = linked_list->first;
    elem_t *next_elem;

    while(current_elem)
    {
        next_elem = current_elem->next;

        if(condition(current_elem, arg))
        {
            return current_elem;
        }

        current_elem = next_elem;
    }

    return NULL;
}

int linked_list_remove_if(linked_list_t *linked_list, const condition_t condition, void *arg)
{
    assert("Linked list cannot be NULL" && linked_list);
    assert("Condition cannot be NULL" && condition);

    elem_t *actual_elem = linked_list->first;
    elem_t *next_elem;

    while(actual_elem)
    {
        next_elem = actual_elem->next;

        if(condition(actual_elem, arg))
        {
            if(linked_list_remove(linked_list, actual_elem))
            {
                return_failure_int;
            }
        }

        actual_elem = next_elem;
    }

    return EXIT_SUCCESS;
}

int linked_list_for_each(const linked_list_t *linked_list, const action_t action, void *arg)
{
    assert("Linked list cannot be NULL" && linked_list);
    assert("Action cannot be NULL" && action);

    elem_t *current_elem = linked_list->first;

    while(current_elem)
    {
        if(action(current_elem, arg))
        {
            return_failure_int;
        }

        current_elem = current_elem->next;
    }

    return EXIT_SUCCESS;
}

static inline void swap_elems_data(elem_t *elem_a, elem_t *elem_b)
{
    void *tmp_data = elem_a->data;
    elem_a->data = elem_b->data;
    elem_b->data = tmp_data;
}

void linked_list_insertion_sort(linked_list_t *linked_list, const condition_t condition, void *arg)
{
    assert("Linked list cannot be NULL" && linked_list);
    assert("Condition cannot be NULL" && condition);

    elem_t *actual_elem = linked_list->first;
    elem_t *next_elem;

    while(actual_elem->next)
    {
        next_elem = actual_elem->next;

        if(condition(actual_elem, arg))
        {
            swap_elems_data(actual_elem, next_elem);
        }

        actual_elem = next_elem;
    }
}
