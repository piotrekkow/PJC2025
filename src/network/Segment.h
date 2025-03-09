#pragma once
#include <utility>
#include "Leg.h"

class Segment
{
	std::pair<Intersection*, Intersection*> m_intersections;
	std::pair<Leg*, Leg*> m_legs; 
	Graph& m_graph;
	// std::pair<Vector2, Vector2> m_bezierHandles; // for curve controls

public:
	Segment(Graph& graph, Leg* leg1, Leg* leg2);

private:
	void linkVertices();
};