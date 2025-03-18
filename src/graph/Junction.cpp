#include "Junction.h"

Junction::Waypoint(Vector2 position) : Vertex(position) {}

std::vector<Edge*> Junction::in() const
{
	return m_in;
}

std::vector<Edge*> Junction::out() const
{
	return m_out;
}

bool Waypoint::addIn(Edge* inEdge)
{
	m_in.push_back(inEdge);
	return true;
}

bool Waypoint::addOut(Edge* outEdge)
{
	m_out.push_back(outEdge);
	return true;
}