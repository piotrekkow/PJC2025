#include "QuadBezier.h"

Vector2 QuadBezier::getCurvePoint(float t) const
{
	float u = 1.0f - t;
	float tt = t * t;
	float uu = u * u;
	return {
		uu * m_controlPoints[0].x + 2 * t * u * m_controlPoints[1].x + tt * m_controlPoints[2].x,
		uu * m_controlPoints[0].y + 2 * t * u * m_controlPoints[1].y + tt * m_controlPoints[2].y
	};
}

void QuadBezier::setOfPoints()
{
	m_points.resize(m_subdivisions + 1);
	for (int i = 0; i <= m_subdivisions; ++i)
	{
		float t = static_cast<float>(i) / m_subdivisions;
		m_points[i] = getCurvePoint(t);
	}
}

QuadBezier::QuadBezier(std::array<Vector2, 3> points, int subdivisions) 
	: m_controlPoints{ points }
	, m_subdivisions{ subdivisions }
{
	setOfPoints();
}

QuadBezier::QuadBezier(Vector2 point1, Vector2 point2, Vector2 point3, int subdivisions)
	: QuadBezier(std::array<Vector2, 3>{ point1, point2, point3 }, subdivisions) {
}


void QuadBezier::drawBezier() const
{
	for (int i = 0; i < m_subdivisions; ++i)
	{
		DrawLineV(m_points[i], m_points[i + 1], BLACK);
	}
}

const std::vector<Vector2>& QuadBezier::points() const
{
	return m_points;
}
