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

bool Junction::removeIn(Edge* inEdge)
{
    auto it = std::find(m_in.begin(), m_in.end(), inEdge);
    if (it != m_in.end())
    {
        m_in.erase(it);
        return true;
    }
    return false;
}

bool Junction::removeOut(Edge* outEdge)
{
    auto it = std::find(m_out.begin(), m_out.end(), outEdge);
    if (it != m_out.end())
    {
        m_out.erase(it);
        return true;
    }
    return false;
}