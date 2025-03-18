#include "Waypoint.h"

Waypoint::Waypoint(Vector2 position) : Vertex(position) {}

Edge* Waypoint::in() const
{
	return m_in;
}

Edge* Waypoint::out() const
{
	return m_out;
}

bool Waypoint::addIn(Edge* inEdge)
{
	if (!m_in)
	{
		m_in = inEdge;
		return true;
	}
	return false;
}

bool Waypoint::addOut(Edge* outEdge)
{
	if (!m_out)
	{
		m_out = outEdge;
		return true;
	}
	return false;
}