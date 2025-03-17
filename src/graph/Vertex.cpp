#include "Vertex.h"

Vertex::Vertex(Vector2 position) : m_position{ position } {}

Vector2& Vertex::pos()
{
	return m_position;
}