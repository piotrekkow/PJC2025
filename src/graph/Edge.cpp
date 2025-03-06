#include "Edge.h"

Edge::Edge(Vertex* destination)
	: m_destination{ destination }
{}

const Vertex* Edge::getDestination() const { return m_destination; }
