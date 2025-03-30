#pragma once
#include <utility>
#include "Network.h"
#include "Edge.h"

class Node;
class Edge;

class Segment
{
	std::vector<std::unique_ptr<Edge>> m_edges;
	Node* m_source;
	Node* m_destination;

public:
	/**
	 * @brief Simple segment constructor for when both nodes have equal sizes
	 * @param source Source node
	 * @param destination Destination node
	 */
	Segment(Node* source, Node* destination);

	/**
	 * @brief Segment constructor for when node sizes differ
	 * @param source Source node
	 * @param destination Destination node
	 * @param isOffset Is the first edge of the larger node offset
	 * 
	 * If the segment is offset then we check for colinnearity between vertex 1 of the larger node and vertex 0 of the smaller node
	 * isOffset == true:
	 * 2[]---[]1	1[]---[]2		 /-[]2	 2[]-\
	 * 1[]---[]0	0[]---[]1	1[]-//-[]1	 1[]-\\-[]1
	 * 0[]				  []0	0[]-/  []0	 0[]  \-[]0
	 * src	 dst	src	  dst	src	   dst	 src	dst
	 * 
	 * isOffset == false:
	 * 2[]  /-[]1	1[]-\  []2	 	   []2	 2[]
	 * 1[]-//-[]0	0[]-\\-[]1	 1[]---[]1	 1[]---[]1
	 * 0[]-/			 \-[]0	 0[]---[]0	 0[]---[]0
	 * src	  dst	src	   dst	 src   dst	 src   dst
	 * 
	 * Can handle at most n->n+2 or n->n-2 when isSourceOffset=true
	 */		
	Segment(Node* source, Node* destination, bool isSourceOffset);

	std::vector<Edge*> getEdges() const;
	Node* getSource() const;
	Node* getDestination() const;

private:
	Edge* addEdge(Vertex* source, Vertex* destination);
};