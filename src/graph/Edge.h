#pragma once

class Vertex;

/**
* Reprezentacja krawedzi w grafie
* Kazda krawedz przechowuje wskaznik wierzcholka do ktorego wchodzi
*/
class Edge
{
	Vertex* m_destination;
	// float speedLimit;

public:
	Edge(Vertex* destination);
	const Vertex* getDestination() const;
};

