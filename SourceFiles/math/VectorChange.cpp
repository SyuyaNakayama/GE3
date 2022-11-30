#include "VectorChange.h"

Vector2 VectorChange(Vector3 vec) { return Vector2(vec.x, vec.y); }
Vector3 VectorChange(Vector2 vec) { return Vector3(vec.x, vec.y, 0); }