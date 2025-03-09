#include "Leg.h"
#include "Intersection.h"

#include <iostream>

Leg::Leg(Graph& graph, Intersection* intersection, Vector2 offset, int laneCount, int inletCount)
	: m_graph{ graph }
	, m_parent{ intersection }
	, m_position{ intersection->getPos() + offset }
	, m_offset{ offset }
	, m_angle{ Angle(offset) }
{
	fillLeg(laneCount, inletCount);
}

const std::vector<Vertex*> Leg::getVertices() const { return m_vertices; }

Intersection* Leg::getParent() const { return m_parent; }

void Leg::drawAxes()
{
	DrawLineV(m_position, m_position + 20 * tangent(), BLUE);
	DrawLineV(m_position, m_position + 20 * normal(), RED);
}

Vector2 Leg::tangent() { return Vector2Normalize(m_offset); }

Vector2 Leg::normal() {
	Vector2 n = { -m_offset.y, m_offset.x };
	return Vector2Normalize(n);
}

void Leg::fillLeg(int laneCount, int inletCount)
{
	Vector2 cursorPos{ getStartingPos(laneCount, LANE_WIDTH) };
	for (int i = 0; i < laneCount; ++i)
	{
		Vertex* newVertex{ m_graph.addVertex(cursorPos, false) };
		if (i < inletCount)
			newVertex->setIntersectionInlet();

		m_vertices.push_back(newVertex);
		cursorPos += normal() * LANE_WIDTH;
	}
}

Vector2 Leg::getStartingPos(int laneCount, float laneWidth)
{
	if (laneCount < 2 || laneWidth <= 0) return m_position;
	float legWidth{ (laneCount - 1) * laneWidth };		// legWidth nie bierze pod uwage szerokosci calej drogi, tylko szerokosc rozkladu wierzcholkow
	return m_position - (normal() * (legWidth / 2));
}

void Leg::moveLeg(float offset)
{
	m_position += tangent() * offset;
	for (auto& vertex : m_vertices)
		vertex->setPos(vertex->getPos() + tangent() * offset);
}

Angle Leg::getAngle()
{
	return m_angle;
}