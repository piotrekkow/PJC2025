#include "Network.h"

Vertex* Network::addVertex(Vector2 position)
{
	auto vertex = std::make_unique<Vertex>(position);
	Vertex* ptr = vertex.get();
	m_vertices.push_back(std::move(vertex));	// unique_ptr vertex cannot be copied - only moved
	return ptr;
}

Edge* Network::addEdge(Vertex* source, Vertex* destination)
{
	if (!source || !destination)
	{
		return nullptr;
	}
	auto edge = std::make_unique<Edge>(source, destination);
	Edge* ptr = edge.get();
	m_edges.push_back(std::move(edge));
	source->addOut(ptr);
	destination->addIn(ptr);
	return ptr;
}

Node* Network::addNode(Vector2 position, int laneCount, Vector2 tangent)
{
	auto node = std::make_unique<Node>(*this, position, laneCount, tangent);
	Node* ptr = node.get();
	m_nodes.push_back(std::move(node));	// unique_ptr vertex cannot be copied - only moved
	return ptr;
}
/*
Segment* Network::addSegment()
{

}
*/

void Network::registerVehicle(Vehicle* vehicle, Edge* edge)
{
    m_edgeVehicleMap[edge].push_back(vehicle);
}

void Network::unregisterVehicle(Vehicle* vehicle, Edge* edge)
{
    auto& vehicles = m_edgeVehicleMap[edge];
    vehicles.erase(std::remove(vehicles.begin(), vehicles.end(), vehicle), vehicles.end());
}

void Network::updateVehicleEdge(Vehicle* vehicle, Edge* oldEdge, Edge* newEdge)
{
    unregisterVehicle(vehicle, oldEdge);
    registerVehicle(vehicle, newEdge);
}

std::vector<Vehicle*> Network::getVehiclesOnEdge(Edge* edge) const
{
    auto it = m_edgeVehicleMap.find(edge);
    if (it != m_edgeVehicleMap.end()) {
        return it->second;
    }
    return {}; // Empty vector if no vehicles on this edge
}

void Network::draw(bool debug)
{
    for (auto& vertex : m_vertices)
    {
        for (auto& edge : vertex->out())
        {
            Vector2 start = vertex->pos();
            Vector2 end = edge->dest()->pos();

            // Draw the main line
            DrawLineV(start, end, EDGE_COLOR);

            // Calculate arrow head
            Vector2 direction = Vector2Normalize(end - start);

            // Create the two arrow wings
            float arrowSize = 10.0f;
            float arrowAngle = 30.0f * DEG2RAD; // 30 degrees in radians

            // Rotate direction vector for left and right wings
            Vector2 leftWing = {
                direction.x * cosf(arrowAngle) - direction.y * sinf(arrowAngle),
                direction.x * sinf(arrowAngle) + direction.y * cosf(arrowAngle)
            };

            Vector2 rightWing = {
                direction.x * cosf(-arrowAngle) - direction.y * sinf(-arrowAngle),
                direction.x * sinf(-arrowAngle) + direction.y * cosf(-arrowAngle)
            };

            // Scale and negate the wing vectors
            leftWing = -arrowSize * leftWing;
            rightWing = -arrowSize * rightWing;

            // Offset the arrow slightly from the destination
            Vector2 arrowBase = end - direction * VERTEX_RADIUS;

            // Draw the two wings of the arrow
            DrawLineV(arrowBase, arrowBase + leftWing, EDGE_COLOR);
            DrawLineV(arrowBase, arrowBase + rightWing, EDGE_COLOR);
        }

        DrawCircleV(vertex->pos(), VERTEX_RADIUS, VERTEX_COLOR);
    }

    if (debug)
    {
        for (auto& node : m_nodes)
        {
            node->drawAxes();
        }
    }
}

auto Network::nodes() const {
	return m_nodes | std::views::transform([](const auto& node) { return node.get(); });
}

