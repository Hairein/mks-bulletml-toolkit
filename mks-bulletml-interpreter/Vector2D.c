#include <math.h>

#include "Vector2D.h"

void init_vector2d(Vector2D* vector) {
    *vector = (Vector2D){0};
}

float Vector2DDotProduct(Vector2D vector1, Vector2D vector2) {
    return (vector1.x * vector2.x + vector1.y * vector2.y);
}

Vector2D Vector2DNormalize(Vector2D vector)
{
    Vector2D result = (Vector2D){0};

    float length = sqrtf((vector.x * vector.x) + (vector.y * vector.y));
    if(length == 0) return result;

    result.x = vector.x / length;
    result.y = vector.y / length;

    return result;
}

Vector2D Vector2DSubtract(Vector2D vector1, Vector2D vector2)
{
    return (Vector2D){vector1.x - vector2.x, vector1.y - vector2.y};
}

Vector2D Vector2DAdd(Vector2D vector1, Vector2D vector2)
{
    return (Vector2D){vector1.x + vector2.x, vector1.y + vector2.y};
}

Vector2D Vector2DScale(Vector2D vector, float scale)
{
    return (Vector2D){vector.x * scale, vector.y * scale};
}

Vector2D Vector2DRotate(Vector2D vector, float angle)
{
    Vector2D result = {0};

    float cosres = cosf(angle);
    float sinres = sinf(angle);

    result.x = vector.x * cosres - vector.y * sinres;
    result.y = vector.x * sinres + vector.y * cosres;

    return result;
}
