#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <SDL3/SDL_rect.h>
#include <math.h>

/**
 * @brief Point struct. (int)
 *
 * @param x     X pos
 * @param y     Y pos
*/
typedef SDL_Point vector2d_t;

/**
 * @brief Point struct. (float)
 *
 * @param x     X pos
 * @param y     Y pos
*/
typedef SDL_FPoint fvector2d_t;

/**
 * @brief Rectangle struct. (int)
 *
 * @param x     X pos
 * @param y     Y pos
 * @param w     Width
 * @param h     Height
*/
typedef SDL_Rect rect_t;

/**
 * @brief Rectangle struct. (float)
 *
 * @param x     X pos
 * @param y     Y pos
 * @param w     Width
 * @param h     Height
*/
typedef SDL_FRect frect_t;

#define EPSILON 0.000001f

#define f_is_zero(X) (fabsf(X) <= EPSILON)

#define f_is_not_zero(X) (fabsf(X) > EPSILON)

void rect_to_frect(const rect_t *rect, frect_t *frect);

void frect_to_rect(const frect_t *frect, rect_t *rect);

/**
 * @brief Verify if a point is in a rectangle. (int)
 *
 * @param P     Vector2d
 * @param R     Rectangle
 *
 * @return True if the point is in the rectangle, else false.
*/
#define point_in_rect(P, R) SDL_PointInRect(P, R)

/**
 * @brief Verify if a point is in a rectangle. (float)
 *
 * @param P     Vector2d
 * @param R     Rectangle
 *
 * @return True if the point is in the rectangle, else false.
*/
#define fpoint_in_frect(P, R) SDL_PointInRectF(P, R)

/**
 * @brief Verify if a rectangle is empty
 *
 * @param R     Rectangle
 *
 * @return True if the rectangle is empty, else false.
*/
#define rect_empty(R) SDL_RectEmpty(R)

/**
 * @brief Verify if a rectangle is empty
 *
 * @param R     Rectangle
 *
 * @return True if the rectangle is empty, else false.
*/
#define frect_empty(R) SDL_FRectEmpty(R)

/**
 * @brief Verify if there is a intersction between two rectangles. (int)
 *
 * @param A     Rectangle A
 * @param B     Rectangle B
 *
 * @return True if there is an intersection, else false.
*/
#define has_intersection(A, B) SDL_HasRectIntersection(A, B)

/**
 * @brief Verify if there is a intersction between two rectangles. (float)
 *
 * @param A     Rectangle A
 * @param B     Rectangle B
 *
 * @return True if there is an intersection, else false.
*/
#define fhas_intersection(A, B) SDL_HasRectIntersectionFloat(A, B)

/**
 * @brief Verify if there is an intersection between two rectangles and get the intersection rectangle. (int)
 *
 * @param A         Rectangle A
 * @param B         Rectangle B
 * @param result    The intersection rectangle
 *
 * @return True if there is an intersection, else false.
*/
#define intersect_rect(A, B, result) SDL_GetRectIntersection(A, B, result)

/**
 * @brief Verify if there is an intersection between two rectangles and get the intersection rectangle. (float)
 *
 * @param A         Rectangle A
 * @param B         Rectangle B
 * @param result    The intersection rectangle
 *
 * @return True if there is an intersection, else false.
*/
#define intersect_frect(A, B, result) SDL_GetRectIntersectionFloat(A, B, result)

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