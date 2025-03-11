#include "Vertex.h"

Vertex::Vertex(Vector2 position) : m_position{ position } {}

Vector2& Vertex::pos()
{
	return m_position;
}

void Vertex::addIn(Edge* inEdge)
{
	m_in.push_back(inEdge);
}

void Vertex::addOut(Edge* outEdge)
{
	m_out.push_back(outEdge);
}

const std::vector<Edge*>& Vertex::out()
{
	return m_out;
}