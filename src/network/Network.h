#pragma once

#include "Intersection.h"
#include "Segment.h"
#include "Graph.h"

class Network
{
	Graph& m_graph;
	std::vector<std::unique_ptr<Intersection>> m_intersections;
	std::vector<std::unique_ptr<Segment>> m_segments;

public:
	Network(Graph& graph);

	Intersection* addIntersection(Vector2 position);
	Segment* addSegment(Leg* leg1, Leg* leg2);
	void addRoad(Vector2 p1, Vector2 p2);
	void addRoad(Vector2 p, Intersection* is2);
	void addRoad(Intersection* is1, Vector2 p);
	void addRoad(Intersection* is1, Intersection* is2);
	void printVertices();
	void draw(bool debug);
};

