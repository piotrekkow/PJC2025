#pragma once
#include "Node.h"

class Node;

class Network
{
	/** We can only store the nodes in the network because each node owns the underyling Segments, Vertices and Edges */
	std::vector<std::unique_ptr<Node>> m_nodes;

	/** Intersections are stored separately because they represent much more complex connections between nodes when compared to Segments */
	// std::vector<std::unique_ptr<Intersection>> m_intersections;

public:
	/**
	 * @brief Adds a node independent of all other nodes in the network
	 * @param position Position of the node
	 * @param tangent Tangent of the node, eg the direction it's facing: useful for node connection logic (curved segments, are nodes colinnear, etc)
	 * @param size Number of lanes (vertices in a node)
	 * @return Newly created node
	 */
	Node* addNode(Vector2 position, Vector2 tangent, int size);

	/**
	 * @brief Adds a node/s and segment/s connecting to that node
	 * If the two nodes representing the beginning and the end of the road are not collinear more nodes and segments may be created to create a curved road 
	 * @param previousNode Pointer to node from which the segment/s to the new node will be created
	 * @param position Position of the node
	 * @param tangent Tangent of the node, eg the direction it's facing: useful for node connection logic (curved segments, are nodes colinnear, etc)
	 * @param size Number of lanes (vertices in a node)
	 * @return All nodes comprising the new road
	 */
	// std::vector<Node*> addRoad(Node* previousNode, Vector2 position, Vector2 tangent, int size);

	/**
	 * @brief Adds a segment connecting two nodes 
	 * @param source Pointer to node from which the segment/s will be created
	 * @param destination Pointer to node to which the segment/s will be created
	 * @return All nodes comprising the new road
	 */
	// std::vector<Node*> addRoad(Node* source, Node* destination);

	/**
	 * @brief Adds a node/s and segment/s connecting to that node
	 * If the two nodes representing the beginning and the end of the road are not collinear more nodes and segments may be created to create a curved road
	 * @param nextNode Pointer to node to which the segment/s from the new node will be created
	 * @param position Position of the node
	 * @param tangent Tangent of the node, eg the direction it's facing: useful for node connection logic (curved segments, are nodes colinnear, etc)
	 * @param size Number of lanes (vertices in a node)
	 * @return All nodes comprising the new road
	 */
	// std::vector<Node*> addRoad(Vector2 position, Vector2 tangent, int size, Node* nextNode);

	// Intersection* addIntersection(std::vector<Node*> inlets, std::vector<Node*> outlets, 
	void draw(bool debug);
private:

};

