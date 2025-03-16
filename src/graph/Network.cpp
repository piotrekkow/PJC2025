#include "Network.h"
#include "QuadBezier.h"

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

void Network::addEdge(Vertex* source, Vertex* destination, int curveSubdiv, Vector2 inTangent, Vector2 outTangent)
{
    Vector2 intersection;
    if (lineIntersect(source->pos(), inTangent, destination->pos(), outTangent, intersection))
    {
        QuadBezier curve(source->pos(), intersection, destination->pos(), curveSubdiv);
        for (auto& point : curve.points())
        {
            if (point == curve.points().front())
            {
                Vertex* newVertex{ addVertex(point) };
                addEdge(source, newVertex);
            }
            else if (point == curve.points().back())
            {
                addEdge(m_vertices.back().get(), destination);
            }
            else
            {
                Vertex* lastVertex{ m_vertices.back().get()};
                Vertex* newVertex{ addVertex(point) };
                addEdge(lastVertex, newVertex);
            }
        }
    }
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
void Network::registerVehicle(Vehicle* vehicle)
{
    if (vehicle->edge())
        m_edgeVehicleMap[vehicle->edge()].push_back(vehicle);
}

void Network::unregisterVehicle(Vehicle* vehicle)
{
    auto& vehicles = m_edgeVehicleMap[vehicle->edge()];
    vehicles.erase(std::remove(vehicles.begin(), vehicles.end(), vehicle), vehicles.end());
}

void Network::drawArrow(Vector2& start, Vector2& end, float lineWidth, Color color)
{
    DrawLineEx(start, end, lineWidth, color);

    Vector2 direction = Vector2Normalize(end - start);

    float arrowSize = 10.0f;
    float arrowAngle = 30.0f * DEG2RAD;

    Vector2 leftWing = {
        direction.x * cosf(arrowAngle) - direction.y * sinf(arrowAngle),
        direction.x * sinf(arrowAngle) + direction.y * cosf(arrowAngle)
    };

    Vector2 rightWing = {
        direction.x * cosf(-arrowAngle) - direction.y * sinf(-arrowAngle),
        direction.x * sinf(-arrowAngle) + direction.y * cosf(-arrowAngle)
    };

    leftWing = -arrowSize * leftWing;
    rightWing = -arrowSize * rightWing;

    Vector2 arrowBase = end - direction * VERTEX_RADIUS;

    DrawLineEx(arrowBase, arrowBase + leftWing, lineWidth, color);
    DrawLineEx(arrowBase, arrowBase + rightWing, lineWidth, color);
}

/*
std::vector<Vehicle*> Network::getVehiclesOnEdge(Edge* edge) const
{
    auto it = m_edgeVehicleMap.find(edge);
    if (it != m_edgeVehicleMap.end()) {
        return it->second;
    }
    return {}; // Empty vector if no vehicles on this edge
}
*/
void Network::draw(bool debug)
{
    for (auto& vertex : m_vertices)
    {
        for (auto& edge : vertex->out())
        {
            Vector2 start = vertex->pos();
            Vector2 end = edge->dest()->pos();
            drawArrow(start, end, 2.0f, EDGE_COLOR);
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

