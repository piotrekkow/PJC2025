#include "Vertex.h"

Vertex::Vertex(Vector2 position) : m_position{ position } {}

Vector2& Vertex::getPos()
{
	return m_position;
}