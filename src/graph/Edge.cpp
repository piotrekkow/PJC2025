#include "Edge.h"
#include "utils.h"


Edge::Edge(Vertex* source, Vertex* destination)
	: m_source{ source }
	, m_destination{ destination }
{
	m_length = getLength();
}

float Edge::getLength()
{
	return Vector2Distance(m_source->pos(), m_destination->pos());
}

Vertex* Edge::dest()
{
	return m_destination;
}