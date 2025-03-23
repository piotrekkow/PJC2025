#pragma once
#include <memory>
#include <unordered_map>
#include <iostream>
#include "Vertex.h"
#include "Node.h"
#include "VehicleGenerator.h"
#include "Vehicle.h"
#include "Waypoint.h"
#include "Junction.h"

class Node;
class Vehicle;

class Network
{
	std::unordered_map<int, std::shared_ptr<Edge>> m_edges;
	std::unordered_map<int, std::unique_ptr<Vertex>> m_vertices;
	std::unordered_map<int, std::unique_ptr<Node>> m_nodes;

	std::unordered_map<Edge*, std::vector<Vehicle*>> m_edgeVehicleMap;
	int m_nextVertId{ 0 }, m_nextEdgeId{ 0 }, m_nextNodeId{ 0 };

	template<typename VertexType, typename... Args>
	int createVertex(Args&&... args)
	{
		auto vertex = std::make_unique<VertexType>(m_nextVertId, std::forward<Args>(args)...);
		m_vertices[m_nextVertId] = std::unique_ptr<Vertex>(vertex.release());
		std::cout << "Added vertex " << m_nextVertId << '\n';
		return m_nextVertId++;
	}

public:
	int addWaypoint(Vector2 position);
	int addJunction(Vector2 position);
	int addEdge(int sourceId, int destinationId); // edge between two existing vertices
	std::vector<int> addEdgeEx(int sourceId, int destinationId); // edge between two existing vertices checking for and adding junctions when crossing existing edges
	//std::vector<Edge*> addEdges(Node* sources, Node* destinations);
	//Edge* addEdge(Vertex* source, Vertex* destination, int curveSubdiv, Vector2 inTangent, Vector2 outTangent); // adds a set of edges and vertices along a quadratic bezier curve, returns final edge
	int addNode(Vector2 position, int laneCount, Vector2 tangent);
	// std::vector<int> addConnection(int sourceId, int destinationId);

	std::vector<int> addConnection(Vertex* source, Vertex* destination);	// straight connection between two existing vertices taking into account possible intersections with other edges
	std::vector<int> addConnection(Vertex* source, Vertex* destination, Vector2& inTangent, Vector2& outTangent, std::optional<int> curveSubdivisions);	// curved connection between two vertices, made up from multiple edges and vertices
	std::vector<int> addConnection(Node* source, Node* destination, std::optional<int> curveSubdivisions); // connection between two nodes, possibly curved if tangents misallign

	

	void draw(bool debug);
	//auto nodes() const;
	
	//void registerVehicle(Vehicle* vehicle);
	//void unregisterVehicle(Vehicle* vehicle);

private:
	void drawArrow(Vector2& start, Vector2& end, float lineWidth, Color color);
	int checkForEdge(Vertex* source, Vertex* destination);
	bool removeEdge(int edgeId);
	int convertToJunction(int waypointId);
	int convertToWaypoint(int junctionId);
};

