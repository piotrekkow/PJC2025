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

Junction* Network::convertToJunction(Waypoint* waypoint)
{
	auto junction = std::make_unique<Junction>(waypoint->pos());
	Junction* jctPtr = junction.get();
	
	if (waypoint->in())
	{
		jctPtr->addIn(waypoint->in());
		waypoint->in()->updateDestination(jctPtr);
	}
	if (waypoint->out())
	{
		jctPtr->addOut(waypoint->out());
		waypoint->out()->updateSource(jctPtr);
	}
	
	// Remove from waypoints container
    auto it = std::remove_if(m_waypoints.begin(), m_waypoints.end(),
        [waypoint](const std::unique_ptr<Waypoint>& wp) {
            return wp.get() == waypoint;
        });
    
    if (it != m_waypoints.end()) {
        m_waypoints.erase(it, m_waypoints.end());
    }
}

Edge* Network::addEdge(Vertex* source, Vertex* destination)
{
	if (!source || !destination)
	{
		return nullptr;
	}
	for (const auto& edge : m_edges)
	{
		if (edge->src() == source && edge->dest() == destination)
		{
			return edge.get();
		}
		if (edge->src() == destination && edge->dest() == source)
		{
			// remove edge
			// add edge in opposite direction
		}
	}

	auto edge = std::make_unique<Edge>(source, destination);
	Edge* edgePtr = edge.get();

	bool srcSuccess{ source->addOut(edge) };
	bool destSuccess{ destination->addIn(edge) };

	if (!srcSuccess)
	{
		Waypoint* waypoint{ static_cast<Waypoint*>(source) };
        Junction* junction = convertToJunction(waypoint);
        edgePtr->updateSource(junction);
        junction->addOut(edgePtr);
	}

	if (!destSuccess)
	{
		Waypoint* waypoint{ static_cast<Waypoint*>(source) };
		Junction* junction{ convertToJunction(waypoint) };
		edgePtr->updateDestination(junction);
		junction->addIn(edgePtr);
	}

	m_edges.push_back(std::move(edge));
	return edgePtr;
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

