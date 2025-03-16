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

bool lineIntersect(const Vector2& v1, const Vector2& tangent1, const Vector2& v2, const Vector2& tangent2, Vector2& intersection)
{
    float det = tangent1.x * tangent2.y - tangent1.y * tangent2.x;
    if (std::abs(det) < 1e-5f)
    {
        return false; // lines parallel
    }
        
    Vector2 diff{ v2 - v1 };
    float s{ diff.x * tangent2.y - diff.y * tangent2.x };
    intersection = v1 + tangent1 * s;
    return true;
}
