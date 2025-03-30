#pragma once
#include <raylib.h>
#include <optional>

Vector2 operator+(const Vector2& v1, const Vector2& v2);
Vector2 operator-(const Vector2& v1, const Vector2& v2);
Vector2 operator*(const Vector2& v, float scalar);
Vector2 operator*(float scalar, const Vector2& v);
Vector2 operator/(const Vector2& v, float scalar);
Vector2& operator+=(Vector2& v1, const Vector2& v2);
Vector2& operator-=(Vector2& v1, const Vector2& v2);
float vector2Length(const Vector2& v);
float vector2Distance(const Vector2& v1, const Vector2& v2);
Vector2 vector2Normalize(const Vector2& v);
bool operator==(const Vector2& v1, const Vector2& v2);
bool operator!=(const Vector2& v1, const Vector2& v2);
Vector2 normalizedTangent(const Vector2& v1, const Vector2 v2);
Vector2 operator-(const Vector2& v);
std::optional<Vector2> lineIntersect(const Vector2& v1, const Vector2& tangent1, const Vector2& v2, const Vector2& tangent2);	// checks the whole line, optional because lines might not intersect
std::optional<Vector2> lineIntersectCap(const Vector2& p1, const Vector2& p2, const Vector2& q1, const Vector2& q2);			// capped at points
float dotProduct(const Vector2& v1, const Vector2& v2);
float determinant(const Vector2& v1, const Vector2& v2);
bool isCollinear(const Vector2& pos1, const Vector2& tan1, const Vector2& pos2, const Vector2& tan2, const float allignmentThreshold);

//temp
void drawArrow(Vector2& start, Vector2& end, float lineWidth, Color color);