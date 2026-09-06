#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>

/**
 * @brief Element struct for linked lists.
 * This struct is the base for storing data in a linked list.
 *
 * @param data  Raw pointer to any type of data
 * @param prev  Pointer to the prev element in the linked list
 * @param next  Pointer to the next element in the linked list
*/
typedef struct elem_t {
    void *data;
    struct elem_t *prev;
    struct elem_t *next;
} elem_t;

/**
 * @brief A type for allocators used during the creation of data stored in elements' data.
 *
 * During the initialisation of linked lists you can pass in argument an allocator.
 * You can pass NULL to the initialisation to not use an allocator.
 * Your allocator_t function declaration must look like this :
 *
 *  int name_of_your_allocator(void* data, va_list args);
 *
 * Then do what you need to fill your data.
 *
 * @param data  Data to fill.
 *
 * @return EXIT_SUCCESS, EXIT_FAILURE on error.
*/
typedef int (*allocator_t)(void *data, va_list args);

/**
 * @brief A type for deleters used during the destruction of data stored in elements' data.
 *
 * During the initialisation of linked lists you can pass in argument a deleter.
 * You can pass NULL to the initialisation to not use a deleter.
 * Your deleter_t function declaration must look like this :
 *
 *  void name_of_your_deleter(void *data);
 *
 * Then do what you need to destroy your data (don't free the data itself).
 *
 * @param data  Data to free.
*/
typedef int (*deleter_t)(void *data);

/**
 * @brief A type for condition used for function like linked_list_remove_if.
 *
 * Your condition_t function declaration must look like this :
 *
 *  bool name_of_your_condition(elem_t *elem, va_list);
 *
 * @param elem  Processed element
 * @param arg   Argument to pass to the function
 *
 * @return False, true for the element to be removed.
*/
typedef bool (*condition_t)(elem_t *elem, void *arg);

/**
 * @brief A type for action used for function like linked_list_for_each
 *
 * Your condition_t function declaration must look like this :
 *
 *  int name_of_your_condition(elem_t *elem, va_list);
 *
 * @param elem  Processed element
 * @param arg   Argument to pass to the function
 *
 * @return EXIT_SUCCESS, EXIT_FAILURE on error.
*/
typedef int (*action_t)(elem_t *elem, void *arg);

/**
 * @brief Base of linked lists.
 *
 * @param data_size         Size of the data stored in elements
 * @param allocator         Allocator of data when creating new elements
 * @param deleter           Deleter of data when removing elements
 * @param first             Pointer to the first element in the list
 * @param last              Pointer to the last element in the list
 * @param size              Size of the linked list
 * @param reserved          Pointer to the first reserved element
 * @param target_max_size   Total target number of elements in the list
*/
typedef struct linked_list_t {
    size_t data_size;
    allocator_t allocator;
    deleter_t deleter;
    elem_t *first;
    elem_t *last;
    size_t size;
    elem_t *reserved;
    size_t target_max_size;
} linked_list_t;

/**
 * @brief Create an elem to insert in a linked list. The data pointer is allocated.
 * If there is an element available in reserved list then it will be used.
 * The element returned is filled with garbage memory.
 *
 * @param linked_list   Pointer to an linked list
 * @param ...           Arguments to pass to the allocator
 *
 * @return pointer to initialised elem, NULL on error.
*/
elem_t *create_elem(linked_list_t *linked_list, ...);

/**
 * @brief Destroy the given element and its data.
 * May move the element into the reserved list for future use depending on target size of the list.
 * This function makes the given element unsafe to use.
 *
 * @param linked_list   Pointer to a linked list
 * @param elem          Pointer to an element
 *
 * @return EXIT_SUCCESS, EXIT_FAILURE on error.
*/
int destroy_elem(linked_list_t *linked_list, elem_t *elem);

/**
 * @brief Init a linked list.
 *
 * @param linked_list   Pointer to a linked list
 * @param data_size     Data size for elements data in the linked list
 * @param deleter       The deleter for elem
*/
void linked_list_init(linked_list_t *linked_list, size_t data_size, const allocator_t allocator, const deleter_t deleter);

/**
 * @brief Clear the given linked list and free reserved elements, do nothing on error.
 * This function makes the given linked list unsafe to use.
 *
 * @brief linked_list   Pointer to a linked list
 *
 * @return EXIT_SUCCESS, EXIT_FAILURE on error.
*/
int linked_list_exit(linked_list_t *linked_list);

/**
 * @brief Pre-allocate elements for a target size.
 * If you want n more elements in your list, then you want linked_list.size + n target_size.
 *
 * @param linked_list   Pointer to a linked list
 * @param target_size   Target size of linked list
 *
 * @return EXIT_SUCCESS, EXIT_FAILURE on error.
 */
int linked_list_reserve(linked_list_t *linked_list, size_t target_size);

/**
 * @brief Clear the given linked list, do nothing on error.
 * May move the elements into the reserved list for future use depending on target size of the list.
 *
 * @brief linked_list   Pointer to a linked list
 *
 * @return EXIT_SUCCESS, EXIT_FAILURE on error.
*/
int linked_list_clear(linked_list_t *linked_list);

/**
 * @brief Insert an element from the back in a linked list, do nothing on error.
 *
 * @param linked_list   Pointer to a linked list
 * @param elem          Pointer to an element
*/
void linked_list_push_back(linked_list_t *linked_list, elem_t *elem);

/**
 * @brief Insert an element from the front in a linked list, do nothing on error.
 *
 * @param linked_list   Pointer to a linked list
 * @param elem          Pointer to an element
*/
void linked_list_push_front(linked_list_t *linked_list, elem_t *elem);

/**
 * @brief Remove and return the last element in the given linked list, do nothing on error.
 * The element returned have unsafe `next` and `prev` pointers.
 *
 * @param linked_list   Pointer to a linked list
 *
 * @return Pointer to element, NULL on error.
*/
elem_t *linked_list_pop_back(linked_list_t *linked_list);

/**
 * @brief Remove and return the first element in the given linked list, do nothing on error.
 * The element returned have unsafe `next` and `prev` pointers.
 *
 * @param linked_list   Pointer to a linked list
 *
 * @return Pointer to element, NULL on error.
*/
elem_t *linked_list_pop_front(linked_list_t *linked_list);

/**
 * @brief Insert element at the given index in the linked list, do nothing on error.
 *
 * @param linked_list   Pointer to a linked list
 * @param elem          Pointer to an element
 * @param index         Index to insert element
 *
 * @return EXIT_SUCCESS, EXIT_FAILURE on error.
*/
int linked_list_insert(linked_list_t *linked_list, elem_t *elem, size_t index);

/**
 * @brief Get element at the given index in the linked list.
 *
 * @param linked_list   Pointer to a linked list
 * @param index         Index of the element
 *
 * @return Pointer to element, NULL on error.
*/
elem_t *linked_list_get(const linked_list_t *linked_list, size_t index);

/**
 * @brief Remove the given elem in the linked list, do nothing on error.
 * May move the elements into the reserved list for future use depending on target size of the list.
 * This function makes the given element unsafe to use.
 *
 * @param linked_list   Pointer to a linked list
 * @param elem          Element to remove
 *
 * @return EXIT_SUCCESS, EXIT_FAILURE on error.
*/
int linked_list_remove(linked_list_t *linked_list, elem_t *elem);

/**
 * @brief Return first element that the condition verify.
 *
 * @param linked_list   Pointer to a linked list
 * @param condition     Condition to verify
 * @param ...           Argument to pass to the condition
 *
 * @return Pointer to element, NULL if no element verify the condition or if an error occurred.
*/
elem_t *linked_list_get_if(const linked_list_t *linked_list, const condition_t condition, void *arg);

/**
 * @brief Remove every elements that the condition verify, do nothing on error.
 *
 * @param linked_list   Pointer to an linked list
 * @param condition     Condition to verify
 * @param ...           Argument to pass to the condition
 *
 * @return EXIT_SUCCESS, EXIT_FAILURE on error.
*/
int linked_list_remove_if(linked_list_t *linked_list, const condition_t condition, void *arg);

/**
 * @brief Apply an action to every elements in the list.
 * The action can be anything that doesn't destroy the element.
 *
 * @param linked_list   Pointer to an linked list
 * @param action        Action to apply
 * @param ...           Argument to pass to the action
 *
 * @return EXIT_SUCCESS, EXIT_FAILURE on error.
*/
int linked_list_for_each(const linked_list_t *linked_list, const action_t action, void *arg);

/**
 * @brief Do an insertion sort data of the given linked list.
 * The condition return true to swap element's data with next one.
 *
 * @param linked_list   Pointer to an linked list
 * @param action        Action to apply
 *
 * @return EXIT_SUCCESS, EXIT_FAILURE on error.
*/
void linked_list_insertion_sort(linked_list_t *linked_list, const condition_t condition, void *arg);

#endif // LINKED_LIST_H
