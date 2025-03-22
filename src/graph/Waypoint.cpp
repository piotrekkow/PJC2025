#include "Waypoint.h"

Waypoint::Waypoint(int id, Vector2 position) : Vertex(id, position) {}

Edge* Waypoint::in() const
{
	return m_in;
}

Edge* Waypoint::out() const
{
	return m_out;
}

bool Waypoint::canAddIn()
{
	if (m_in) return false;
	return true;
}

bool Waypoint::canAddOut()
{
	if (m_out) return false;
	return true;
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