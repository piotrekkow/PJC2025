#include <raylib.h>
#include <iostream>
#include "Graph.h"

Vertex* Graph::addVertex(Vector2 pos, bool isIntersectionInlet)
{
	auto vertex = std::make_unique<Vertex>(pos, isIntersectionInlet);
	Vertex* ptr = vertex.get();
	m_vertices.push_back(std::move(vertex));	// unique_ptr vertex cannot be copied - only moved
	return ptr;
}

void Graph::addEdge(Vertex* source, Vertex* destination)
{
	if (source->isValidDestination(destination))
		source->getEdges().emplace_back(destination);
	else
		std::cout << "Edge already exists.\n";
}

void Graph::draw()
{
	for (const auto& vertex : m_vertices)
	{
		// draw edges
		for (const auto& edge : vertex->getEdges())
			DrawLineV(vertex->getPos(), edge.getDestination()->getPos(), EDGE_COLOR);
		// draw vertices
		DrawCircleV(vertex->getPos(), VERTEX_RADIUS, VERTEX_COLOR);
	}
}