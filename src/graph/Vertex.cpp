#include "Vertex.h"

Vertex::Vertex(Vector2 position, bool isIntersectionInlet)
	: m_position{ position }
	, m_isIntersectionInlet{ isIntersectionInlet }
{}

Vector2 Vertex::getPos() const { return m_position; }
std::vector<Edge>& Vertex::getEdges() { return m_edges; }	// may be used in the future for removing edges
const std::vector<Edge>& Vertex::getEdges() const { return m_edges; }
const bool Vertex::isIntersectionInlet() const { return m_isIntersectionInlet; }
const bool Vertex::isSegmentInlet() const { return !m_isIntersectionInlet; }
void Vertex::setIntersectionInlet() { m_isIntersectionInlet = true; }
void Vertex::setSegmentInlet() { m_isIntersectionInlet = false; }
void Vertex::setPos(Vector2 newPosition) { m_position = newPosition; }

bool Vertex::isValidDestination(Vertex* destination)	// check if the edge already exists
{
	for (const auto& edge : m_edges) // const reference to ensure no changes
	{
		if (edge.getDestination() == destination) { return false; }
	}
	return true;
}