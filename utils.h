#include <raylib.h>

Vector2 operator+(const Vector2& v1, const Vector2& v2);
Vector2 operator-(const Vector2& v1, const Vector2& v2);
Vector2 operator*(const Vector2& v, float scalar);
Vector2 operator*(float scalar, const Vector2& v);
Vector2 operator/(const Vector2& v, float scalar);
Vector2& operator+=(Vector2& v1, const Vector2& v2);
Vector2& operator-=(Vector2& v1, const Vector2& v2);
float Vector2Length(const Vector2& v);
Vector2 Vector2Normalize(const Vector2& v);