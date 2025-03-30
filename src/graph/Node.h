#pragma once
#include "Vertex.h"
#include "Network.h"
#include "Segment.h"


class Segment;

class Node
{
	/** Vertices represent points to whcih lanes (edges) are attached to in a node */
	std::vector<std::unique_ptr<Vertex>> m_vertices;

	/** Segment outgoing from this node. This node owns this segment. There can only be one such segment. Doesn't have to exist. */
	std::unique_ptr<Segment> m_out;

	/** Segment incoming into this node. This node does not own this segment. There can only be one such segment. Doesn't have to exist. */
	Segment* m_in;

	/** Center point of the node */
	Vector2 m_position;

	/** Direction the node is facing, tangent to the incoming and outgoing edges */
	Vector2 m_tangent;

public:

	/**
	 * @brief Node constructor
	 * @param position Position of the node
	 * @param tangent Tangent of the node, eg the direction it's facing: useful for node connection logic (curved segments, are nodes colinnear, etc)
	 * @param size Number of lanes (vertices in a node)
	 */
	Node(Vector2 position, Vector2 tangent, int size);

	Vector2 getPos() const;
	Vector2 getTangent() const;
	size_t getSize() const;
	std::vector<Vertex*> getVertices() const;
	Segment* getOut() const;
	Segment* getIn() const;

	/**
	 * @brief Adds a segment from this node to destination node
	 * @param destination Destination node
	 * @return Returns added segment
	 */
	Segment* addOutSegment(Node* destination);

	/** Sets an m_in segment - this node is the destination for that segment. That segment is owned by another node. */
	void setInSegment(Segment* inSegment);

private:

	/** Normal represents the direction in which the vertices will be placed when constructing a node */
	Vector2 normal();

	template<typename T>
	Vertex* addVertex(Vector2 position)
	{
		std::unique_ptr<T> vertex = std::make_unique<T>(position);
		m_vertices.emplace_back(std::move(vertex));
		return m_vertices.back().get();
	}

	/** Fills node with vertices evenly spaced out along normal(), centered around m_position */
	std::vector<Vertex*> fillNode(int size);
};