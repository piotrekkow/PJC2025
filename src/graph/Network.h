#pragma once
#include <memory>
#include "Vertex.h"
#include "Node.h"


class Node;

class Network
{
	std::vector<std::unique_ptr<Edge>> m_edges;
	std::vector<std::unique_ptr<Vertex>> m_vertices;
	std::vector<std::unique_ptr<Node>> m_nodes;
	// std::vector<std::unique_ptr<Segment>> m_segments;

public:
	Vertex* addVertex(Vector2 position);
	Edge* addEdge(Vertex* source, Vertex* destination);
	Node* addNode(Vector2 position, int laneCount, Vector2 tangent);
	// Segment* addSegment();
	void draw(bool debug);
	auto nodes() const;
};

