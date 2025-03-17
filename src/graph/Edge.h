#pragma once
#include "Vertex.h"

class Vertex;

class Edge
{
	Vertex* m_source;
	Vertex* m_destination;
	float m_length;

public:
	Edge(Vertex* source, Vertex* destination);
	const float length() const;
	Vertex* dest() const;
	Vertex* src() const;
	const Vector2 tangent() const;
	void updateSource(Vertex* newSource);
	void updateDestination(Vertex* newDestination);
};