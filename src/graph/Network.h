#pragma once
#include <memory>
#include <unordered_map>
#include "Vertex.h"
#include "Node.h"
#include "VehicleGenerator.h"
#include "Vehicle.h"

class Node;

class Network
{
	std::vector<std::unique_ptr<Edge>> m_edges;
	std::vector<std::unique_ptr<Vertex>> m_vertices;
	std::vector<std::unique_ptr<Node>> m_nodes;
	// std::vector<std::unique_ptr<Segment>> m_segments;
	std::vector<VehicleGenerator> m_generators;

	std::unordered_map<Edge*, std::vector<Vehicle*>> m_edgeVehicleMap;

public:
	Vertex* addVertex(Vector2 position);
	Edge* addEdge(Vertex* source, Vertex* destination);
	Node* addNode(Vector2 position, int laneCount, Vector2 tangent);
	// Segment* addSegment();
	void draw(bool debug);
	auto nodes() const;
	
	void registerVehicle(Vehicle* vehicle, Edge* edge);
 void unregisterVehicle(Vehicle* vehicle, Edge* edge);
 void updateVehicleEdge(Vehicle* vehicle, Edge* oldEdge, Edge* newEdge);
 std::vector<Vehicle*> getVehiclesOnEdge(Edge* edge) const;
 void checkCollisions();
};

