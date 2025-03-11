#pragma once

#include <vector>
#include <raylib.h>
#include "Edge.h"

class Edge;

class Vertex
{
	std::vector<Edge*> m_in;
	std::vector<Edge*> m_out;
	Vector2 m_position;

public:
	Vertex(Vector2 position);
	Vector2& pos();

	void addIn(Edge* inEdge);
	void addOut(Edge* outEdge);
	const std::vector<Edge*>& out();
};