#include "Network.h"

#include <iostream>

Network::Network(Graph& graph) : m_graph{ graph } {}

Intersection* Network::addIntersection(Vector2 position)
{
	auto intersection = std::make_unique<Intersection>(m_graph, position);
	Intersection* ptr = intersection.get();
	m_intersections.push_back(std::move(intersection));
	return ptr;
}

Segment* Network::addSegment(Leg* leg1, Leg* leg2)
{
	auto segment = std::make_unique<Segment>(m_graph, leg1, leg2);
	Segment* ptr = segment.get();
	m_segments.push_back(std::move(segment));
	return ptr;
}

void Network::addRoad(Vector2 p1, Vector2 p2)
{
	Vector2 nTangent{ Vector2Normalize(p2 - p1) };
	Intersection* is1{ addIntersection(p1) };
	Intersection* is2{ addIntersection(p2) };
	Leg* leg1{ is1->addLeg(nTangent * LEG_OFFSET, 2, 1) };
	Leg* leg2{ is2->addLeg(-nTangent * LEG_OFFSET, 2, 1) };
	addSegment(leg1, leg2);
}
void Network::addRoad(Vector2 p, Intersection* is2)
{
	Vector2 nTangent{ Vector2Normalize(is2->getPos() - p) };
	Intersection* is1{ addIntersection(p) };
	Leg* leg1{ is1->addLeg(nTangent * LEG_OFFSET, 2, 1) };
	Leg* leg2{ is2->addLeg(-nTangent * LEG_OFFSET, 2, 1) };
	addSegment(leg1, leg2);
}
void Network::addRoad(Intersection* is1, Vector2 p)
{
	Vector2 nTangent{ Vector2Normalize(p - is1->getPos()) };
	Intersection* is2{ addIntersection(p) };
	Leg* leg1{ is1->addLeg(nTangent * LEG_OFFSET, 2, 1) };
	Leg* leg2{ is2->addLeg(-nTangent * LEG_OFFSET, 2, 1) };
	addSegment(leg1, leg2);
}
void Network::addRoad(Intersection* is1, Intersection* is2)
{
	Vector2 nTangent{ Vector2Normalize(is2->getPos() - is1->getPos()) };
	Leg* leg1{ is1->addLeg(nTangent * LEG_OFFSET, 2, 1) };
	Leg* leg2{ is2->addLeg(-nTangent * LEG_OFFSET, 2, 1) };
	addSegment(leg1, leg2);
}

void Network::printVertices()
{
	std::cout << '1';
	for (auto& intersection : m_intersections)
		for (auto& leg : intersection->getLegs())
			for (auto& vertex : leg->getVertices())
				if (vertex)
					std::cout << "Vertex " << vertex << " at " << vertex->getPos().x << ',' << vertex->getPos().y << ", inlet = " << vertex->isIntersectionInlet() << '\n';
				else
					std::cout << "empty leg" << '\n';
}

void Network::draw(bool debug)
{
	for (auto& intersection : m_intersections) {
		Vector2 isSize{ 40, 40 };
		DrawRectangleV(intersection->getPos() - isSize/2, isSize, LIGHTGRAY);
		for (auto& leg : intersection->getLegs())
		{
			for (auto& vertex : leg->getVertices())
			{
				for (auto& edge : vertex->getEdges())
					DrawLineV(vertex->getPos(), edge.getDestination()->getPos(), EDGE_COLOR);
				DrawCircleV(vertex->getPos(), VERTEX_RADIUS, VERTEX_COLOR);
				if (vertex->isIntersectionInlet())
					DrawCircleV(vertex->getPos(), VERTEX_RADIUS - 2, BACKGROUND_COLOR);
			}
			if (debug)
				leg->drawAxes();
		}
	}
}