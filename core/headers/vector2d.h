/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
*   All of the functions declared in this file and          *
*   defined in vector2d.c are from the SDL2 source code.    *
*   They are just modified for float and renamed.           *
*    => https://github.com/libsdl-org/SDL                   *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Point struct.
 *
 * @param x     X pos
 * @param y     Y pos
*/
typedef struct vector2d_t {
    float x, y;
} vector2d_t;

/**
 * @brief Rectangle struct.
 *
 * @param x     X pos
 * @param y     Y pos
 * @param w     Width
 * @param h     Height
*/
typedef struct rect_t {
    float x, y;
    float w, h;
} rect_t;

#define EPSILON 0.000001f

__attribute__((const)) float fabsf(float x);


#define is_equal_to_zero(X) (fabsf(X) <= EPSILON)

#define is_not_zero(X) (fabsf(X) > EPSILON)

/**
 * @brief Verify if a point is in a rectangle
 *
 * @param P     Vector2d
 * @param R     Rectangle
 *
 * @return True if the point is in the rectangle, else false.
*/
#define point_in_rect(P, R) (((P)->x >= (R)->x) && ((P)->x < ((R)->x + (R)->w)) && \
                             ((P)->y >= (R)->y) && ((P)->y < ((R)->y + (R)->h)))

/**
 * @brief Verify if a rectangle is empty
 *
 * @param R     Rectangle
 *
 * @return True if the rectangle is empty, else false.
*/
#define rect_empty(R) ((!(R)) || (is_equal_to_zero((R)->w)) || (is_equal_to_zero((R)->h)))

/**
 * @brief Verify if there is a intersction between two rectangles
 *
 * @param A     Rectangle A
 * @param B     Rectangle B
 *
 * @return True if there is an intersection, else false.
*/
bool has_intersection(const rect_t *A, const rect_t *B);

/**
 * @brief Verify if there is an intersection between two rectangles and get the intersection rectangle.
 *
 * @param A         Rectangle A
 * @param B         Rectangle B
 * @param result    The intersection rectangle
 *
 * @return True if there is an intersection, else false.
*/
bool intersect_rect(const rect_t *A, const rect_t *B, rect_t *result);

#endif // VECTOR2D_H