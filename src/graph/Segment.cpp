#include "Segment.h"

//Segment::Segment(Network& network, Vector2 nodeA, Vector2 nodeB, int laneCount)
//	: m_network{ network }
//{
//	m_nodeA = m_network.addNode(nodeA, laneCount, normalizedTangent(nodeA, nodeB));
//	m_nodeB = m_network.addNode(nodeB, laneCount, normalizedTangent(nodeB, nodeA));
//
//	for (int i = 0; i < laneCount; ++i)
//	{
//		m_network.addEdge(m_nodeA->vertices()[i], m_nodeB->vertices()[laneCount - i - 1]);
//	}
//}
