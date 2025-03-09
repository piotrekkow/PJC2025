#pragma once

#include "Graph.h"
#include "utils.h"
#include "Angle.h"

class Intersection;

class Leg
{
	std::vector<Vertex*> m_vertices;
	Vector2 m_offset;
	Vector2 m_position;
	Angle m_angle;	// angle relative to x axis
	Graph& m_graph;
	Intersection* m_parent;		// pozwala znalezc roznice pozycji odnogi i srodka skrzyzowania - do wektora stycznego do segmentu
	const float m_minOffset{ 20 };

public:
	Leg(Graph& graph, Intersection* intersection, Vector2 offset, int laneCount, int inletCount);
	const std::vector<Vertex*> getVertices() const;
	Intersection* getParent() const;
	void drawAxes();
	void moveLeg(float offset); // offset along tangent
	Angle getAngle();

private:
	Vector2 tangent();
	Vector2 normal();
	void fillLeg(int laneCount, int inletCount);
	Vector2 getStartingPos(int laneCount, float laneWidth);

};