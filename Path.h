#pragma once
#include "Edge.h"

class Path
{
	std::vector<Edge*> m_edges;
	Vertex* m_source;
	Vertex* m_destination;
	float m_length;
};

