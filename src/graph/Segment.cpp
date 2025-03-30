#include "Segment.h"
#include <stdexcept>

Segment::Segment(Node* source, Node* destination)
	: m_source{ source }
	, m_destination{ destination }
{
	if (m_source->getSize() != m_destination->getSize()) throw std::logic_error("Connecting nodes must be of equal size");
	for (size_t i = 0; i < m_source->getSize(); ++i)
	{
		addEdge(m_source->getVertices()[i], m_destination->getVertices()[i]);
	}
}

Segment::Segment(Node* source, Node* destination, bool isOffset)
	: m_source{ source }
	, m_destination{ destination }
{
	size_t srcSize{ m_source->getSize() };
	size_t dstSize{ m_destination->getSize() };
	int sizeDifference{ static_cast<int>(srcSize) - static_cast<int>(dstSize) };

	if (std::abs(sizeDifference) > 2)
	{
		throw std::logic_error("Size difference between connected Nodes must not exceed 2");
	}
	else if (sizeDifference == 0)
	{
		Segment(source, destination);
	}

	if (sizeDifference > 0) // source larger than destination
	{
		for (size_t i = 0; i < m_destination->getSize(); ++i)
		{
			if (isOffset)
				addEdge(m_source->getVertices()[i + 1], m_destination->getVertices()[i]);
			else
				addEdge(m_source->getVertices()[i], m_destination->getVertices()[i]);
		}
	}
	else // source smaller than destination
	{
		for (size_t i = 0; i < m_source->getSize(); ++i)
		{
			if (isOffset)
				addEdge(m_source->getVertices()[i], m_destination->getVertices()[i + 1]);
			else
				addEdge(m_source->getVertices()[i], m_destination->getVertices()[i]);
		}
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
