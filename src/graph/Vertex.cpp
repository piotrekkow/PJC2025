#include "Vertex.h"

Vertex::Vertex(int id, Vector2 position)
	: m_id{ id }
	, m_position{ position } {}

const int Vertex::id() const
{
	return m_id;
}

Vector2& Vertex::pos()
{
	return m_position;
}