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
	// constexpr float m_minAngleBetweenLegs{ 45 };

public:
	Intersection(Graph& graph, Vector2 position);
	const Vector2 getPos() const;
	Leg* addLeg(Vector2 offset, unsigned laneCount, unsigned inletCount);
	void connectLegs();

private:
	/*
	bool isValidDirection(Vector2 direction);
	Vector2 newLegOffset(Vector2 direction, int laneCount);
	*/
};

