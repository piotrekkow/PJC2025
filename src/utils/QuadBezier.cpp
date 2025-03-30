#include "QuadBezier.h"
#include "utils.h"
#include <stdexcept>

QuadBezier::QuadBezier(std::array<Vector2, 3> controlPoints, int subdivisions) 
	: m_controlPoints{ controlPoints }
	, m_subdivisions{ subdivisions }
{
	setOfPoints();
}

QuadBezier::QuadBezier(Vector2 start, Vector2 control, Vector2 end, int subdivisions)
	: QuadBezier(std::array<Vector2, 3>{ start, control, end }, subdivisions)
{}

QuadBezier::QuadBezier(std::array<Vector2, 3> controlPoints, float allignmentThreshold)
	: m_controlPoints{ controlPoints }
	, m_subdivisions{ calculateRequiredSubdivisions(allignmentThreshold) }
{
	setOfPoints();
}

QuadBezier::QuadBezier(Vector2 start, Vector2 control, Vector2 end, float allignmentThreshold)
	: QuadBezier(std::array<Vector2, 3>{ start, control, end }, allignmentThreshold)
{}


void QuadBezier::drawBezier() const
{
	for (int i = 0; i < m_subdivisions; ++i)
	{
		DrawLineV(m_points[i], m_points[i + 1], BLACK);
	}
}

const std::vector<Vector2>& QuadBezier::getPoints() const
{
	return m_points;
}

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

Vector2 QuadBezier::getCurveTangent(float t) const
{
	Vector2 tangent
	{
		2 * (1 - t) * (m_controlPoints[1].x - m_controlPoints[0].x) + 2 * t * (m_controlPoints[2].x - m_controlPoints[1].x),
		2 * (1 - t) * (m_controlPoints[1].y - m_controlPoints[0].y) + 2 * t * (m_controlPoints[2].y - m_controlPoints[1].y)
	};
	return vector2Normalize(tangent);
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
int QuadBezier::calculateRequiredSubdivisions(float allignmentThreshold)
{
	int subdivisions{ 2 };
	const int limit{ 20 };
	bool allSegmentsCollinear{ false };

	while (subdivisions < limit)
	{
		for (int i = 0; i < subdivisions; ++i)
		{
			float t1{ static_cast<float>(i) / subdivisions };
			float t2{ static_cast<float>(i + 1) / subdivisions };

			Vector2 p1{ getCurvePoint(t1) };
			Vector2 p2{ getCurvePoint(t2) };
			Vector2 tan1{ getCurveTangent(t1) };

			if (isCollinear(p1, tan1, p2, tan1, allignmentThreshold))
				allSegmentsCollinear = true;

			if (allSegmentsCollinear)
				return subdivisions;

			subdivisions += 2;
		}
	}
	throw std::logic_error("Subdivided past limit");
}
