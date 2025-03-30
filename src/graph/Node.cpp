#include "Node.h"
#include "utils.h"
#include "config.h"
#include "Waypoint.h"
// #include "Junction.h"
#include "QuadBezier.h"
#include <stdexcept>
#include <iostream>

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
	int srcSize{ static_cast<int>(m_vertices.size()) };
	int dstSize{ static_cast<int>(destination->getSize()) };
	int sizeDifference{ srcSize - dstSize };
	

	if (sizeDifference == 0)
	{
		bool collinear{ isCollinear(m_position, m_tangent, destination->getPos(), destination->getTangent(), 0.98f) };
		if (collinear) // straight edge case
		{
			m_out = std::make_unique<Segment>(this, destination);
			std::cout << "Creating straight segment\n";
		}
		// THIS CASE SHOULD BE IN NETWORK:
		else // curve case (add multiple subdivisions in between)
		{
			std::optional<Vector2> intersectionOpt{ lineIntersect(m_position, m_tangent, destination->getPos(), destination->getTangent()) };
			if (intersectionOpt.has_value())
			{
				Vector2 intersection{ intersectionOpt.value() };
				QuadBezier curve(m_position, intersection, destination->getPos(), 0.98f);
				for (auto& point : curve.getPoints())
				{
					if (point == curve.getPoints().begin()) break;
					if (point == curve.getPoints().end())
				}
				std::cout << "Creating curved segment\n";
			}
		}
	}


	destination->setInSegment(m_out.get());
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
	Vector2 cursor{ m_position };
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