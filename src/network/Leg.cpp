#include "Leg.h"
#include "Intersection.h"

Leg::Leg(Graph& graph, Intersection* intersection, Vector2 offset, int laneCount, int inletCount)
	: m_graph{ graph }
	, m_parent{ intersection }
	, m_position{ intersection->getPos() + offset }
	, m_offset{ offset }
{
	fillLeg(laneCount, inletCount);
}

const std::vector<Vertex*> Leg::getVertices() const { return m_vertices; }

Intersection* Leg::getParent() const { return m_parent; }

Vector2 Leg::tangent() { return Vector2Normalize(m_offset); }


void Leg::fillLeg(int laneCount, int inletCount)
{
	Vector2 cursorPos{ getStartingPos(laneCount, LANE_WIDTH) };
	for (int i = 0; i < laneCount; ++i)
	{
		if (i < inletCount)
			m_graph.addVertex(cursorPos, true);
		else
			m_graph.addVertex(cursorPos, false);
	}
}

Vector2 Leg::getStartingPos(int laneCount, float laneWidth)
{
	if (laneCount < 2 || laneWidth <= 0) return m_position;
	float legWidth{ (laneCount - 1) * laneWidth };		// legWidth nie bierze pod uwage szerokosci calej drogi, tylko szerokosc rozkladu wierzcholkow
	return m_position - (tangent() * (legWidth / 2));
}