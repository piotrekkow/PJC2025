#include "Edge.h"
#include "utils.h"


Edge::Edge(Vertex* source, Vertex* destination)
	: m_source{ source }
	, m_destination{ destination }
{
	m_length = length();
}

const float Edge::length() const
{
	return Vector2Distance(m_source->pos(), m_destination->pos());
}

Vertex* Edge::dest() const
{
	return m_destination;
}

Vertex* Edge::src() const
{
	return m_source;
}

const Vector2 Edge::tangent() const
{
	return normalizedTangent(m_source->pos(), m_destination->pos());
}
