#include "Segment.h"
#include <stdexcept>

Segment::Segment(Node* source, Node* destiantion)
	: m_source{ source }
	, m_destination{ destiantion }
{
	if (m_source->getSize() != m_destination->getSize()) throw std::logic_error("Connecting nodes must be of equal size");
	for (size_t i = 0; i < m_source->getSize(); ++i)
	{
		addEdge(m_source->getVertices()[i], m_destination->getVertices()[i]);
	}
}

std::vector<Edge*> Segment::getEdges() const
{
	std::vector<Edge*> edges{};
	for (auto& edge : m_edges)
	{
		edges.emplace_back(edge.get());
	}
	return edges;
}

Node* Segment::getSource() const
{
	return m_source;
}

Node* Segment::getDestination() const
{
	return m_destination;
}

Edge* Segment::addEdge(Vertex* source, Vertex* destination)
{
	std::unique_ptr<Edge> edge = std::make_unique<Edge>(source, destination);
	m_edges.emplace_back(std::move(edge));
	return m_edges.back().get();
}
