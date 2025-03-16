#pragma once
#include <utility>
#include "Network.h"

class Segment
{
	Node* m_nodeA;
	Node* m_nodeB;
	Network& m_network;

public:
	Segment(Network& network, Vector2 nodeA, Vector2 nodeB, int laneCount);
	// Segment(Node* nodeA, Node* nodeB, Vector2 ctrlPointB, Vector2 controlPointB);
	// void fillSegment(int subdivisions);

private:
	
};