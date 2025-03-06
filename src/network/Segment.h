#pragma once
#include <utility>
#include "Leg.h"

class Segment
{
	std::pair<Intersection*, Intersection*> m_intersections;
	std::pair<Leg*, Leg*> m_legs;
	// std::pair<Vector2, Vector2> m_bezierHandles; // for curve controls

public:
	Segment(Leg* leg1, Leg* leg2);
};

