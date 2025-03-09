#pragma once

#include <vector>
#include <memory>
#include "Vertex.h"
#include "config.h"

/**
* Graf to niskopoziomowa reprezentacja symulowanej sieci drogowej.
* Sama klasa przechowuje tylko wierzcholki, informacja o polaczeniach znajduje sie w wierzcholku.
*/
class Graph
{
	std::vector<std::unique_ptr<Vertex>> m_vertices;

public:
	Vertex* addVertex(Vector2 pos, bool isIntersectionInlet);
	void addEdge(Vertex* source, Vertex* destination);
	void draw();
};
