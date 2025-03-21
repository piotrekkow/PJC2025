#include "Junction.h"

Junction::Junction(int id, Vector2 position) : Vertex(id, position) {}

std::vector<Edge*> Junction::in() const
{
	return m_in;
}

std::vector<Edge*> Junction::out() const
{
	return m_out;
}

bool Junction::canAddIn()
{
	return true; // Junctions can have multiple incoming edges
}

bool Junction::canAddOut()
{
	return true; // Junctions can have multiple outgoing edges
}

bool Junction::addIn(Edge* inEdge)
{
	m_in.push_back(inEdge);
	return true;
}

bool Junction::addOut(Edge* outEdge)
{
	m_out.push_back(outEdge);
	return true;
}