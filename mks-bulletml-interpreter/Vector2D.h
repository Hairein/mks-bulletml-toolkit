#ifndef VECTOR2D_H
#define VECTOR2D_H

typedef struct {
    float x;
    float y;
} Vector2D;

void init_vector2d(Vector2D* vector2d);

float Vector2DDotProduct(Vector2D vector1, Vector2D vector2);
Vector2D Vector2DNormalize(Vector2D vector2d);
Vector2D Vector2DSubtract(Vector2D vector1, Vector2D vector2);
Vector2D Vector2DAdd(Vector2D vector1, Vector2D vector2);
Vector2D Vector2DScale(Vector2D vector, float scale);
Vector2D Vector2DRotate(Vector2D vector, float angle);

#endif // VECTOR2D_H
