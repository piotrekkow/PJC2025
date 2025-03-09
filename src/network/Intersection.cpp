#include "Intersection.h"
#include "Leg.h"
#include "Graph.h"

Intersection::Intersection(Graph& graph, Vector2 position)
	: m_graph{ graph }
	, m_position{ position }
{}

const Vector2 Intersection::getPos() const { return m_position; }

Leg* Intersection::addLeg(Vector2 offset, int laneCount, int inletCount)
{
	auto leg = std::make_unique<Leg>(m_graph, this, offset, laneCount, inletCount);
	Leg* ptr = leg.get();
	m_legs.push_back(std::move(leg));
	return ptr;
}

const std::vector<Leg*> Intersection::getLegs() const {
	std::vector<Leg*> legs;
	legs.reserve(m_legs.size());
	for (const auto& legPtr : m_legs) {
		legs.push_back(legPtr.get());
	}
	return legs;
}

void Intersection::connectLegs()
{
	for (auto& currentLeg : m_legs)
	{
		for (auto& otherLeg : m_legs)
		{
			if (otherLeg != currentLeg)
			{
				for (auto& inVertex : currentLeg->getVertices())
				{
					if (inVertex->isIntersectionInlet())
					{
						for (auto& outVertex : otherLeg->getVertices())
						{
							if (outVertex->isSegmentInlet())
								m_graph.addEdge(inVertex, outVertex);
						}
					}
				}
			}
		}
	}
}

/*
bool Intersection::isValidDirection(Vector2 direction)
{
	for (auto& leg : m_legs)
		if (getAbsAngle(direction, leg->getOffset()) < m_minAngleBetweenLegs) // needs to be implemented
			return false;
	return true;
}

Vector2 Intersection::newLegOffset(Vector2 direction, int laneCount)
{
	if (!isValidDirection(direction)) return // error
}
*/