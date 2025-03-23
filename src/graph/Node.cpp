#include "Node.h"

Vector2 Node::tangent() const
{
	return m_tangent;
}

int Node::size() const
{
	return m_laneCount;
}

Vector2 Node::normal() { return { -m_tangent.y, m_tangent.x }; }

void Node::fillNode()
{
	Vector2 step{ normal() * LANE_WIDTH };
	Vector2 cursor{ m_position - step * static_cast<float>(m_laneCount - 1) / 2 };
	for (int i = 0; i < m_laneCount; ++i)
	{
		m_network.addWaypoint(cursor);
		cursor += step;
	}
}

Node::Node(Network& network, Vector2 position, int laneCount, Vector2 tangent)
	: m_network{ network }
	, m_position{ position }
	, m_tangent{ Vector2Normalize(tangent) }
	, m_laneCount{ laneCount }
{
	fillNode();
}

Vector2 Node::pos() const
{
	return m_position;
}

void Node::drawAxes()
{
	DrawLineV(m_position, m_position + m_tangent * DEBUG_AXES_LENGTH, TANGENT_COLOR);
	DrawLineV(m_position, m_position + normal() * DEBUG_AXES_LENGTH, NORMAL_COLOR);
}