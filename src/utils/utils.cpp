#include <cmath> // sqrt
#include <algorithm> // max
#include "utils.h"

Vector2 operator+(const Vector2& v1, const Vector2& v2)
{
    return { v1.x + v2.x, v1.y + v2.y };
}

Vector2 operator-(const Vector2& v1, const Vector2& v2)
{
    return { v1.x - v2.x, v1.y - v2.y };
}

Vector2 operator*(const Vector2& v, float scalar)
{
    return { v.x * scalar, v.y * scalar };
}

Vector2 operator*(float scalar, const Vector2& v)
{
    return { v.x * scalar, v.y * scalar };
}

Vector2 operator/(const Vector2& v, float scalar)
{
    if (scalar != 0.0f)
        return { v.x / scalar, v.y / scalar };
    return v;
}

Vector2& operator+=(Vector2& v1, const Vector2& v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    return v1;
}

Vector2& operator-=(Vector2& v1, const Vector2& v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
    return v1;
}

float Vector2Length(const Vector2& v)
{
    return std::sqrt(v.x * v.x + v.y * v.y);
}

float Vector2Distance(const Vector2& v1, const Vector2& v2)
{
    return std::sqrt((v2.x-v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y));
}

Vector2 Vector2Normalize(const Vector2& v)
{
    float length = Vector2Length(v);
    if (length != 0.0f)
        return { v.x / length, v.y / length };
    return v;
}

bool operator==(const Vector2& v1, const Vector2& v2) 
{
    const float precision = 0.00001f; //! precyzja e-5

    //! porównywane wartoœci bliskie zeru
    if (std::abs(v1.x) < precision && std::abs(v2.x) < precision &&
        std::abs(v1.y) < precision && std::abs(v2.y) < precision) {
        return true;
    }

    //! pozosta³e przypadki
    float xEpsilon = std::abs(std::max(std::abs(v1.x), std::abs(v2.x)) * precision);
    float yEpsilon = std::abs(std::max(std::abs(v1.y), std::abs(v2.y)) * precision);

    return (std::abs(v1.x - v2.x) < xEpsilon) &&
        (std::abs(v1.y - v2.y) < yEpsilon);
}

bool operator!=(const Vector2& v1, const Vector2& v2)
{
    return !(v1 == v2);
}

Vector2 normalizedTangent(const Vector2& v1, const Vector2 v2)
{
    return Vector2Normalize(v2 - v1);
}

Vector2 operator-(const Vector2& v)
{
    return { -v.x, -v.y };
}

std::optional<Vector2> lineIntersect(const Vector2& v1, const Vector2& tangent1, const Vector2& v2, const Vector2& tangent2)
{
    float det{ determinant(tangent1, tangent2) };
    if (std::abs(det) < 1e-5f)
    {
        return std::nullopt; // lines parallel
    }
        
    Vector2 diff{ v2 - v1 };
    float s{ diff.x * tangent2.y - diff.y * tangent2.x };
    return { v1 + tangent1 * s };
}

std::optional<Vector2> lineIntersectCap(const Vector2& p1, const Vector2& p2, const Vector2& q1, const Vector2& q2)
{
    // direction vectors
    Vector2 r{ p2 - p1 };
    Vector2 s{ q2 - q1 };

    float det{ determinant(r, s) };
    if (std::abs(det) < 1e-5f)
    {
        return std::nullopt; // lines parallel
    }

    Vector2 c = q1 - p1;
    float t = determinant(c, s) / det;
    float u = determinant(c, r) / det;

    // Check if intersection is within both line segments
    if (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f)
    {
        return p1 + r * t; // Return intersection point
    }
    return std::nullopt;
}

float dotProduct(const Vector2& v1, const Vector2& v2)
{
	return { v1.x * v2.x + v1.y * v2.y };
}

float determinant(const Vector2& v1, const Vector2& v2)
{
    return { v1.x * v2.y - v1.y * v2.x };
}

bool isCollinear(const Vector2& pos1, const Vector2& tan1, const Vector2& pos2, const Vector2& tan2, const float allignmentThreshold)
{
	// check normalized vector between two positions against tangents
	// directional (eg. if tan1 and tan2 are opposite then they're not considered collinear by this function)
	Vector2 normTangent{ normalizedTangent(pos1, pos2) };
	float dot1{ dotProduct(normTangent, tan1) };
	float dot2{ dotProduct(normTangent, tan2) };
	return dot1 > allignmentThreshold && dot2 > allignmentThreshold;
}
