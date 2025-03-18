#pragma once

#include <vector>
#include <raylib.h>
#include "Edge.h"

class Edge;

class Vertex
{
protected:
	Vector2 m_position;

public:
	Vertex(Vector2 position);
	virtual ~Vertex() = default;

	Vector2& pos();

	virtual bool addIn(Edge* inEdge) = 0;
	virtual bool addOut(Edge* outEdge) = 0;
};