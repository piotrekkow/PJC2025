#pragma once
#include <utility>
#include "Network.h"

class Node;

class Segment
{
	std::vector<std::unique_ptr<Edge>> m_edges;
	Node* m_source;
	Node* m_destination;

public:
	Segment(Node* source, Node* destiantion);
	std::vector<Edge*> getEdges() const;
	Node* getSource() const;
	Node* getDestination() const;
	
private:
	Edge* addEdge(Vertex* source, Vertex* destination);
};