#pragma once

#include <memory>
#include <raylib.h>
#include "Graph.h"
#include "Leg.h"

class Leg;

class Intersection
{
	std::vector<std::unique_ptr<Leg>> m_legs; // Intersection legs - odnogi skrzyzowania
	Vector2 m_position;
	Graph& m_graph;

public:
	Intersection(Graph& graph, Vector2 position);
	const Vector2 getPos() const;
	const std::vector<Leg*> getLegs() const;
	Leg* addLeg(Vector2 offset, int laneCount, int inletCount);
	void connectLegs();

private:
	/*
	bool isValidDirection(Vector2 direction);
	Vector2 newLegOffset(Vector2 direction, int laneCount);
	*/
};

