#include "Edge.h"
#include "utils.h" //vector2Distance()
#include "config.h" //EDGE_COLOR

Edge::Edge(Vertex* source, Vertex* destination)
	: m_source{ source }
	, m_destination{ destination }
	, m_length{ vector2Distance(m_source->getPos(), m_destination->getPos()) }
{
	m_source->addOut(this);
	m_destination->addIn(this);
}

void Edge::draw()
{
	DrawLineV(m_source->getPos(), m_destination->getPos(), EDGE_COLOR);
}
