#include "Segment.h"

#include <iostream>

Segment::Segment(Graph& graph, Leg* leg1, Leg* leg2)
	: m_graph { graph }
	, m_legs{ leg1, leg2 }
	, m_intersections{ leg1->getParent(), leg2->getParent() }
{
	linkVertices();
}

void Segment::linkVertices()
{
	const std::vector<Vertex*>& leg1Verts{ m_legs.first->getVertices() };
	const std::vector<Vertex*>& leg2Verts{ m_legs.second->getVertices() };
	for (auto& vertex : leg1Verts)
		std::cout << vertex << ':' << vertex->getPos().x << ',' << vertex->getPos().y << '\n';
	for (int i = 0; i < leg1Verts.size(); ++i)
	{
		if (leg1Verts[i]->isSegmentInlet())
			m_graph.addEdge(leg1Verts[i], leg2Verts[leg1Verts.size()-1-i]);
		if (leg2Verts[i]->isSegmentInlet())
			m_graph.addEdge(leg2Verts[i], leg1Verts[leg1Verts.size()-1-i]);
	}
}
