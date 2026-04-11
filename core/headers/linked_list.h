#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

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
 * @brief A type for deleters used during the destruction of data stored in elements.
 *
 * During the initialisation of linked lists you can pass in argument a deleter.
 * It is used during the destruction of an element.
 * You can pass NULL to the initialisation to use the default deleter (kfree(elem->data)).
 * Your deleter_t function declaration must look like this :
 *
 *  void name_of_your_deleter(void *data);
 *
 * Then do what you need to destroy your data (don't forget to free the data itself...) but not the elements.
 * You should always define your function with "inline" if it's no too long.
*/
typedef void (*deleter_t)(void *data);

/**
 * @brief A type for condition used for function like linked_list_remove_if
 *
 * For function like linked_list_remove_if you need a condition.
 * It have two argument which are "elem_t *" and "va_list", that is because it passes the element on the actual index
 * and the args passed to the function.
 * Your condition_t function declaration must look like this :
 *
 *  bool name_of_your_condition(elem_t *elem, va_list);
 *
 * Then return true if you want the element to be removed from the list or false if not.
 * You should always define your function with "static", and "inline" if the condition is not too long.
*/
typedef bool (*condition_t)(elem_t *elem, va_list args);

/**
 * @brief A type for action used for function like linked_list_for_each
 *
 * For function like linked_list_for_each you need an action to do.
 * It have two argument which are "elem_t *" and "va_list", that is because it passes the element on the actual index
 * and the args passed to the function.
 * Your condition_t function declaration must look like this :
 *
 *  void name_of_your_condition(elem_t *elem, va_list);
 *
 * Then do whatever you want with the element data, just don't remove it.
 * You should always define your function with "static", and "inline" if the action is short.
*/
typedef void (*action_t)(elem_t *elem, va_list args);

/**
 * @brief Base of linked lists.
 * This struct is the base for creating linked lists.
 *
 * @param data_size  Size of the data stored in elements of the linked list (used while creating new elements)
 * @param first      Pointer to the first element in the linked list
 * @param last       Pointer to the last element in the linked list
 * @param size       Size of the linked list
 * @param deleter    Deleter of the data when removing an element
*/
typedef struct linked_list_t {
    size_t data_size;
    elem_t *first;
    elem_t *last;
    size_t size;
    deleter_t elem_deleter;
} linked_list_t;

/**
 * @brief Init a linked list
 * This function is necessary if you create a linked list.
 *
 * WARNING : It doesn't allocate the memory for you !
 *
 * @param linked_list   Pointer to an linked list
 * @param data_size     Data size for elements in the linked list
 * @param deleter       The deleter used during destroyement of an elem
*/
void linked_list_init(linked_list_t *linked_list, const size_t data_size, deleter_t deleter);

/**
 * @brief Create an elem to fill with data to insert in a linked list.
 * This function is usefull when you want to initialise an elem with the proper data_size strored in the linked list.
 * 
 * WARNING : Don't change the "data" ptr after calling this function or it will lead to memory leaks !
 * Instead do something like this : *(int *)elem->data = 42;
 *
 * @param linked_list   Pointer to an linked list
 *
 * @return Address of initialised elem, NULL on error.
*/
elem_t *elem_create(const linked_list_t *linked_list);

/**
 * @brief Destroy the given element and its data.
 * This function is usefull when you want to free an element.
 * The deleter is used for the data only and you can pass NULL to use the default deleter.
 *
 * @param elem          Pointer to an element
 * @param elem_deleter  Function to use to free the data
*/
void destroy_elem(elem_t *elem, const deleter_t elem_deleter);

/**
 * @brief Insert an element from the back in a linked list, do nothing on error.
 *
 * @param linked_list   Pointer to a linked list
 * @param elem          Pointer to an element
*/
void linked_list_push_back_elem(linked_list_t *linked_list, elem_t *elem);

/**
 * @brief Insert an element from the front in a linked list, do nothing on error.
 *
 * @param linked_list   Pointer to a linked list
 * @param elem          Pointer to an element
*/
void linked_list_push_front_elem(linked_list_t *linked_list, elem_t *elem);

/**
 * @brief Insert an element initialised and filled with data from the back in a linked list, do nothing on error.
 *
 * @param linked_list   Pointer to a linked list
 * @param data          Pointer to any data
*/
void linked_list_push_back(linked_list_t *linked_list, void *data);

/**
 * @brief Insert an element filled with data from the front in a linked list.
 *
 * @param linked_list   Pointer to a linked list
 * @param data          Pointer to any data
*/
void linked_list_push_front(linked_list_t *linked_list, void *data);

/**
 * @brief Delete the last element in the given linked list, do nothing on error.
 *
 * @param linked_list   Pointer to a linked list
*/
void linked_list_pop_back(linked_list_t *linked_list);

/**
 * @brief Delete the first element in the given linked list, do nothing on error.
 *
 * @param linked_list   Pointer to a linked list
*/
void linked_list_pop_front(linked_list_t *linked_list);

/**
 * @brief Clear the given linked list, do nothing on error.
 *
 * @brief linked_list   Pointer to a linked list
*/
void linked_list_clear(linked_list_t *linked_list);

/**
 * @brief Check if the given linked list is empty.
 *
 * @param linked_list   Pointer to a linked list
 *
 * @return True if the given linked list is empty else false.
*/
bool linked_list_is_empty(const linked_list_t *linked_list);

/**
 * @brief Check if the given index is whithin the range of the linked list.
 *
 * @param linked_list   Pointer to an linked_list_t
 *
 * @return True if index in within the range of the linked list else false.
*/
bool linked_list_is_in_bound(const linked_list_t *linked_list, const size_t index);

/**
 * @brief Get element at the given index in the linked list.
 *
 * @param linked_list   Pointer to a linked list
 * @param index         Index of the element
 *
 * @return Element, NULL on error.
*/
elem_t *linked_list_get_elem(const linked_list_t *linked_list, const size_t index);

/**
 * @brief Get data in the element at the given index in the linked list.
 *
 * @param linked_list   Pointer to an linked list
 * @param index         Index of the wanted element
 *
 * @return Data, NULL on error.
*/
void *linked_list_get(const linked_list_t *linked_list, const size_t index);

/**
 * @brief Insert element at the given index in the linked list, do nothing on error.
 *
 * @param linked_list   Pointer to a linked list
 * @param elem          Pointer to an element
 * @param index         Index to insert element
*/
void linked_list_insert_elem(linked_list_t *linked_list, elem_t *elem, const size_t index);

/**
 * @brief Insert element with data at the given index in the linked list, do nothing on error.
 *
 * @param linked_list   Pointer to a linked list
 * @param data          Data to put in the new element
 * @param index         Index to insert the element
*/
void linked_list_insert(linked_list_t *linked_list, void *data, const size_t index);

/**
 * @brief Remove the given elem in the linked list, do nothing on error.
 *
 * @param linked_list   Pointer to a linked list
 * @param elem          Element to remove
*/
void linked_list_remove_elem(linked_list_t *linked_list, elem_t *elem);

/**
 * @brief Remove element at the given index in the linked list, do nothing on error.
 *
 * @param linked_list   Pointer to a linked list
 * @param index         Index of the element
*/
void linked_list_remove(linked_list_t *linked_list, const size_t index);

/**
 * @brief Return first element that the condition verify
 *
 * @param linked_list   Pointer to a linked list
 * @param condition     Condition to verify
 * @param ...           Argument to pass to the condition
 *
 * @return Element, NULL if no element verify the condition or if an error ocurred.
*/
elem_t *linked_list_get_elem_if(const linked_list_t *linked_list, const condition_t condition, ...);

/**
 * @brief Return data in the first element that the condition verify
 *
 * @param linked_list   Pointer to a linked list
 * @param condition     Condition to verify
 * @param ...           Argument to pass to the condition
 *
 * @return Element data, NULL if no element verify the condition or if an error ocurred.
*/
void *linked_list_get_if(const linked_list_t *linked_list, const condition_t condition, ...);

/**
 * @brief Remove every elements that the condition verify, do nothing on error.
 *
 * @param linked_list   Pointer to an linked list
 * @param condition     Condition to verify
 * @param ...           Argument to pass to the condition
*/
void linked_list_remove_if(linked_list_t *linked_list, const condition_t condition, ...);

/**
 * @brief Apply an action to every elements in the list.
 * Useful for changing a value in every single element data.
 * The action can be anything that doesn't destroy the element.
 *
 * @param linked_list   Pointer to an linked list
 * @param action        Action to apply
 * @param ...           Argument to pass to the action
*/
void linked_list_for_each(const linked_list_t *linked_list, const action_t action, ...);

#endif // LINKED_LIST_H
