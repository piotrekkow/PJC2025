#include "Node.h"
#include "utils.h"
#include "config.h"
#include "Waypoint.h"
// #include "Junction.h"
#include <stdexcept>

Node::Node(Vector2 position, Vector2 tangent, int laneCount)
	: m_position{ position }
	, m_tangent{ vector2Normalize(tangent) }
{
	fillNode(laneCount);
}


Vector2 Node::getPos() const
{
	return m_position;
}

Vector2 Node::getTangent() const
{
	return m_tangent;
}

size_t Node::getSize() const
{
	return m_vertices.size();
}

std::vector<Vertex*> Node::getVertices() const
{
	std::vector<Vertex*> vertices{};
	for (auto& vertex : m_vertices)
	{
		vertices.emplace_back(vertex.get());
	}
	return vertices;
}

Segment* Node::getOut() const
{
	return m_out.get();
}

Segment* Node::getIn() const
{
	return m_in;
}

Segment* Node::addOutSegment(Node* destination)
{
	if (!isCollinear(m_position, m_tangent, destination->getPos(), destination->getTangent())) throw std::logic_error("Nodes must be collinear to be viably connected by a Segment");

	m_out = std::make_unique<Segment>(this, destination);
	return m_out.get();
}

void Node::setInSegment(Segment* inSegment)
{
	m_in = inSegment;
}

Vector2 Node::normal()
{
	return { -m_tangent.y, m_tangent.x };
}

std::vector<Vertex*> Node::fillNode(int size)
{
	std::vector<Vertex*> vertices{};

	Vector2 step{ normal() * LANE_WIDTH };
	Vector2 cursor{ m_position - step * static_cast<float>(size - 1) / 2 };
	for (int i = 0; i < size; ++i)
	{
		auto vertex{ addVertex<Waypoint>(cursor) };
		vertices.emplace_back(vertex);
		cursor += step;
	}
	return vertices;
}




//void Node::drawAxes()
//{
//	DrawLineV(m_position, m_position + m_tangent * DEBUG_AXES_LENGTH, TANGENT_COLOR);
//	DrawLineV(m_position, m_position + normal() * DEBUG_AXES_LENGTH, NORMAL_COLOR);
//}