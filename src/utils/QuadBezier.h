#pragma once
#include <array>
#include <vector>
#include <raylib.h>

class QuadBezier
{
	const int m_subdivisions;
	std::array<Vector2, 3> m_controlPoints{}; // control points
	std::vector<Vector2> m_points;

public:
	QuadBezier(std::array<Vector2, 3> controlPoints, int subdivisions);
	QuadBezier(Vector2 start, Vector2 control, Vector2 end, int subdivisions);
	QuadBezier(std::array<Vector2, 3> controlPoints, float allignmentThreshold);
	QuadBezier(Vector2 start, Vector2 control, Vector2 end, float allignmentThreshold);

	void drawBezier() const;
	const std::vector<Vector2>& getPoints() const;

private:
	Vector2 getCurvePoint(float t) const;
	Vector2 getCurveTangent(float t) const;
	void setOfPoints();
	int calculateRequiredSubdivisions(float allignmentThreshold);
};

