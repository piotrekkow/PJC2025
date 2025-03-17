#include "Network.h"
#include "QuadBezier.h"
#include <memory>

Waypoint* Network::addWaypoint(Vector2 position)
{
    auto waypoint = std::make_unique<Waypoint>(position);
    Waypoint* ptr = waypoint.get();
    m_waypoints.push_back(std::move(waypoint));
    return ptr;
}

Junction* Network::addJunction(Vector2 position)
{
    auto junction = std::make_unique<Junction>(position);
    Junction* ptr = junction.get();
    m_junctions.push_back(std::move(junction));
    return ptr;
}

Vertex* Network::ensureJunction(Vertex* vertex) {
    // Check if it's already a Junction
    if (dynamic_cast<Junction*>(vertex) != nullptr) {
        return vertex;
    }

    // Need to convert - create a new Junction
    auto waypoint = static_cast<Waypoint*>(vertex);
    Vector2 position = waypoint->pos();

    auto junction = std::make_unique<Junction>(position);
    Junction* junctionPtr = junction.get();
    
    if (waypoint->in())
    {
        junctionPtr->addIn(waypoint->in());
        waypoint->in()->updateDestination(junctionPtr);
    }

    if (waypoint->out())
    {
        junctionPtr->addOut(waypoint->out());
        waypoint->out()->updateSource(junctionPtr);
    }

    m_junctions.push_back(std::move(junction));

    // Remove the waypoint from m_waypoints
    auto it = std::remove_if(m_waypoints.begin(), m_waypoints.end(),
        [waypoint](const std::unique_ptr<Waypoint>& wp){
            return wp.get() == waypoint;
        });

    if (it != m_waypoints.end())
    {
        m_waypoints.erase(it, m_waypoints.end());
    }

    return junctionPtr;
}

Edge* Network::addEdge(Vertex* source, Vertex* destination)
{
	if (!source || !destination)
	{
		return nullptr;
	}
    // Check if source or destination
    if (Waypoint* srcWaypoint = dynamic_cast<Waypoint*>(source))
    {
        if (srcWaypoint->out() != nullptr)
        {
            source = ensureJunction(srcWaypoint);
        }
    }
    if (Waypoint* destWaypoint = dynamic_cast<Waypoint*>(destination))
    {
        if (destWaypoint->in() != nullptr)
        {
            destination = ensureJunction(destWaypoint);
        }
    }

	auto edge = std::make_unique<Edge>(source, destination);
	Edge* ptr = edge.get();
	m_edges.push_back(std::move(edge));
	source->addOut(ptr);
	destination->addIn(ptr);
	return ptr;
}

Edge* Network::addEdge(Vertex* source, Vertex* destination, int curveSubdiv, Vector2 inTangent, Vector2 outTangent)
{
    Vector2 intersection;
    if (lineIntersect(source->pos(), inTangent, destination->pos(), outTangent, intersection))
    {
        QuadBezier curve(source->pos(), intersection, destination->pos(), curveSubdiv);
        Vertex* currentVertex{ source };

        for (size_t i = 1; i < curve.points().size(); ++i)
        {
            const Vector2& point{ curve.points()[i] };
            if (i == curve.points().size() - 1)
            {
                return addEdge(currentVertex, destination);
            }
            else
            {
                Vertex* newVertex{ addVertex(point) };
                addEdge(currentVertex, newVertex);
                currentVertex = newVertex;
            }
        }
    }
    else
    {
        addEdge(source, destination);
    }
    return nullptr;
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

