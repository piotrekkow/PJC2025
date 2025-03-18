#pragma once
#include <memory>
#include <unordered_map>
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
	std::vector<std::unique_ptr<Edge>> m_edges;
	std::vector<std::unique_ptr<Junction>> m_junctions;
	std::vector<std::unique_ptr<Waypoint>> m_waypoints;
	std::vector<std::unique_ptr<Node>> m_nodes;

	std::unordered_map<Edge*, std::vector<Vehicle*>> m_edgeVehicleMap;

public:
	Waypoint* addWaypoint(Vector2 position);
	Junction* addJunction(Vector2 position);
	Junction* convertToJunction(Waypoint* waypoint);
	Edge* addEdge(Vertex* source, Vertex* destination);
	std::vector<Edge*> addEdges(Node* sources, Node* destinations);
	Edge* addEdge(Vertex* source, Vertex* destination, int curveSubdiv, Vector2 inTangent, Vector2 outTangent); // adds a set of edges and vertices along a quadratic bezier curve, returns final edge
	Node* addNode(Vector2 position, int laneCount, Vector2 tangent);

	void draw(bool debug);
	auto nodes() const;
	
	void registerVehicle(Vehicle* vehicle);
	void unregisterVehicle(Vehicle* vehicle);

private:
	void drawArrow(Vector2& start, Vector2& end, float lineWidth, Color color);
};

