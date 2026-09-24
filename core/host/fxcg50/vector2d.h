#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <stdbool.h>
#include <math.h>

/**
 * @brief Point struct. (int)
 *
 * @param x     X pos
 * @param y     Y pos
*/
typedef struct vector2d_t {
    int x, y;
} vector2d_t;

/**
 * @brief Point struct. (float)
 *
 * @param x     X pos
 * @param y     Y pos
*/
typedef struct fvector2d_t {
    float x, y;
} fvector2d_t;

/**
 * @brief Rectangle struct. (int)
 *
 * @param x     X pos
 * @param y     Y pos
 * @param w     Width
 * @param h     Height
*/
typedef struct rect_t {
    int x, y;
    int w, h;
} rect_t;

/**
 * @brief Rectangle struct. (float)
 *
 * @param x     X pos
 * @param y     Y pos
 * @param w     Width
 * @param h     Height
*/
typedef struct frect_t {
    float x, y;
    float w, h;
} frect_t;

#define EPSILON 0.0f

#define f_is_zero(X) (fabsf(X) <= EPSILON)

#define f_is_not_zero(X) (fabsf(X) > EPSILON)

void rect_to_frect(const rect_t *rect, frect_t *frect);

void frect_to_rect(const frect_t *frect, rect_t *rect);

/**
 * @brief Verify if a point is in a rectangle. (int)
 *
 * @param p     Vector2d
 * @param r     Rectangle
 *
 * @return True if the point is in the rectangle, else false.
*/
bool point_in_rect(const vector2d_t *p, const rect_t *r);

/**
 * @brief Verify if a point is in a rectangle. (float)
 *
 * @param p     Vector2d
 * @param r     Rectangle
 *
 * @return True if the point is in the rectangle, else false.
*/
bool fpoint_in_rect(const fvector2d_t *p, const frect_t *r);

/**
 * @brief Verify if a rectangle is empty
 *
 * @param r     Rectangle
 *
 * @return True if the rectangle is empty, else false.
*/
bool rect_empty(const rect_t *r);

/**
 * @brief Verify if a rectangle is empty
 *
 * @param r     Rectangle
 *
 * @return True if the rectangle is empty, else false.
*/
bool frect_empty(const frect_t *r);

/**
 * @brief Verify if there is a intersction between two rectangles. (int)
 *
 * @param A     Rectangle A
 * @param B     Rectangle B
 *
 * @return True if there is an intersection, else false.
*/
bool has_intersection(const rect_t *a, const rect_t *b);

/**
 * @brief Verify if there is a intersction between two rectangles. (float)
 *
 * @param A     Rectangle A
 * @param B     Rectangle B
 *
 * @return True if there is an intersection, else false.
*/
bool fhas_intersection(const frect_t *a, const frect_t *b);

/**
 * @brief Verify if there is an intersection between two rectangles and get the intersection rectangle. (int)
 *
 * @param A         Rectangle A
 * @param B         Rectangle B
 * @param result    The intersection rectangle
 *
 * @return True if there is an intersection, else false.
*/
bool intersect_rect(const rect_t *a, const rect_t *b, rect_t *result);

/**
 * @brief Verify if there is an intersection between two rectangles and get the intersection rectangle. (float)
 *
 * @param A         Rectangle A
 * @param B         Rectangle B
 * @param result    The intersection rectangle
 *
 * @return True if there is an intersection, else false.
*/
bool intersect_frect(const frect_t *a, const frect_t *b, frect_t *result);

/**
 * @brief Lerp a to b with smoothing factor s.
 *
 * @param a Current value
 * @param b Target value
 * @param s Smoothing factor
 */
float lerp(float a, float b, float s);

/**
 * @brief Lerp a to b with smoothing factor s and time basis t.
 * Smoothing factor is for 60 fps call time.
 *
 * @param a Current value
 * @param b Target value
 * @param s Smoothing factor
 * @param t Delta time
 */
float lerp_time(float a, float b, float s, float t);

#endif // VECTOR2D_H