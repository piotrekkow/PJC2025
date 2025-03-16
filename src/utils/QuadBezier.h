#pragma once
#include <array>
#include <vector>
#include <raylib.h>

class QuadBezier
{
	const int m_subdivisions;
	std::array<Vector2, 3> m_controlPoints{}; // control points
	std::vector<Vector2> m_points;

	Vector2 getCurvePoint(float t) const;
	void setOfPoints();

public:
	QuadBezier(std::array<Vector2, 3> points, int subdivisions);
	QuadBezier(Vector2 point1, Vector2 point2, Vector2 point3, int subdivisions);
	void drawBezier() const;
	const std::vector<Vector2>& points() const;
};

