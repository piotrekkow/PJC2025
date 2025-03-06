#include "Segment.h"

Segment::Segment(Leg* leg1, Leg* leg2)
	: m_legs{ leg1, leg2 }
	, m_intersections{ leg1->getParent(), leg2->getParent() }
{}
