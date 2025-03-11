#include "Network.h"

Vertex* Network::addVertex(Vector2 position)
{
	auto vertex = std::make_unique<Vertex>(position);
	Vertex* ptr = vertex.get();
	m_vertices.push_back(std::move(vertex));	// unique_ptr vertex cannot be copied - only moved
	return ptr;
}

Edge* Network::addEdge(Vertex* source, Vertex* destination)
{
	if (!source || !destination)
	{
		return nullptr;
	}
	auto edge = std::make_unique<Edge>(source, destination);
	Edge* ptr = edge.get();
	m_edges.push_back(std::move(edge));
	source->addOut(ptr);
	destination->addIn(ptr);
	return ptr;
}

Node* Network::addNode(Vector2 position, int laneCount, Vector2 tangent)
{
	auto node = std::make_unique<Node>(*this, position, laneCount, tangent);
	Node* ptr = node.get();
	m_nodes.push_back(std::move(node));	// unique_ptr vertex cannot be copied - only moved
	return ptr;
}
/*
Segment* Network::addSegment()
{

}
*/

void Network::draw(bool debug)
{
	for (auto& vertex : m_vertices)
	{
		for (auto& edge : vertex->out())
		{
			DrawLineV(vertex->pos(), edge->dest()->pos(), EDGE_COLOR);
		}
		DrawCircleV(vertex->pos(), VERTEX_RADIUS, VERTEX_COLOR);
	}
	if (debug)
	{
		for (auto& node : m_nodes)
		{
			node->drawAxes();
		}
	}
}

auto Network::nodes() const {
	return m_nodes | std::views::transform([](const auto& node) { return node.get(); });
}