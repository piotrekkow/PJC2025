#pragma once
#include <raylib.h>

Vector2 operator+(const Vector2& v1, const Vector2& v2);
Vector2 operator-(const Vector2& v1, const Vector2& v2);
Vector2 operator*(const Vector2& v, float scalar);
Vector2 operator*(float scalar, const Vector2& v);
Vector2 operator/(const Vector2& v, float scalar);
Vector2& operator+=(Vector2& v1, const Vector2& v2);
Vector2& operator-=(Vector2& v1, const Vector2& v2);
float Vector2Length(const Vector2& v);
float Vector2Distance(const Vector2& v1, const Vector2& v2);
Vector2 Vector2Normalize(const Vector2& v);
bool operator==(const Vector2& v1, const Vector2& v2);
bool operator!=(const Vector2& v1, const Vector2& v2);
Vector2 normalizedTangent(const Vector2& v1, const Vector2 v2);
Vector2 operator-(const Vector2& v);
bool lineIntersect(const Vector2& v1, const Vector2& tangent1, const Vector2& v2, const Vector2& tangent2, Vector2& intersection);
Vector2 dotProduct(const Vector2& v1, const Vector2& v2);
bool isCollinear(const Vector2& pos1, const Vector2& tan1, const Vector2& pos2, const Vector2& tan2, const float allignmentThreshold);