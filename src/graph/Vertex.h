#pragma once

#include <vector>
#include <raylib.h>
#include "Edge.h"

class Vertex
{
	std::vector<Edge> m_edges;
	Vector2 m_position;
	bool m_isIntersectionInlet;

public:
	Vertex(Vector2 position, bool isIntersectionInlet);

	Vector2 getPos() const;
	std::vector<Edge>& getEdges();
	const std::vector<Edge>& getEdges() const;
	const bool isIntersectionInlet() const;
	const bool isSegmentInlet() const;
	void setIntersectionInlet();
	void setSegmentInlet();

	bool isValidDestination(Vertex* destination);
};
