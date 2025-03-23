#include "Network.h"
#include "QuadBezier.h"
#include <memory>
#include <numbers>
#include <algorithm> // std::sort
#include <string>

int Network::addWaypoint(Vector2 position)
{
    return createVertex<Waypoint>(position);
}

int Network::addJunction(Vector2 position)
{
    return createVertex<Junction>(position);
}

int Network::convertToJunction(int waypointId)
{
    auto it = m_vertices.find(waypointId);
    if (it == m_vertices.end()) return -1;
    auto& waypointPtr{ m_vertices[waypointId] };
    Waypoint* waypoint{ dynamic_cast<Waypoint*>(waypointPtr.get()) };
    if (!waypoint)
        return -1;  // not waypoint

    auto junction = std::make_unique<Junction>(waypointId, waypoint->pos());
    Junction* junctionPtr = junction.get();
    
    Edge* inEdge{ waypoint->in() };
    Edge* outEdge{ waypoint->out() };

    m_vertices[waypointId] = std::unique_ptr<Vertex>(junction.release());

    if (inEdge)
    {
        junctionPtr->addIn(inEdge);
        inEdge->updateDestination(junctionPtr);
    }

    if (outEdge)
    {
        junctionPtr->addOut(outEdge);
        outEdge->updateSource(junctionPtr);
    }
    
    std::cout << "Junction " << junctionPtr->id() << " now connected to ";
    for (auto& out : junctionPtr->out())
    {
        std::cout << out->dest()->id() << ' ';
    }
    std::cout << " and from ";
    for (auto& in : junctionPtr->in())
    {
        std::cout << in->src()->id() << ' ';
    }
    std::cout << "\n";
    return waypointId;
}

int Network::convertToWaypoint(int junctionId)
{
    auto it = m_vertices.find(junctionId);
    if (it == m_vertices.end()) return -1;
    auto& junctionPtr{ m_vertices[junctionId] };
    Junction* junction{ dynamic_cast<Junction*>(junctionPtr.get()) };
    if (!junction)
        return -1; // not junction
    if (junction->in().size() > 1 || junction->out().size() > 1) 
        return -1; // cannot turn a junction serving multiple edges into waypoint 

    auto waypoint = std::make_unique<Waypoint>(junctionId, junction->pos());
    Waypoint* waypointPtr = waypoint.get();
    
    Edge* inEdge{ junction->in()[0] };
    Edge* outEdge{ junction->out()[0] };

    m_vertices[junctionId] = std::unique_ptr<Vertex>(waypoint.release());

    if (inEdge)
    {
        junctionPtr->addIn(inEdge);
        inEdge->updateDestination(waypointPtr);
    }

    if (outEdge)
    {
        junctionPtr->addOut(outEdge);
        outEdge->updateSource(waypointPtr);
    }

    return junctionId;
}

int Network::addEdge(int sourceId, int destinationId)
{
    Vertex* source{ m_vertices[sourceId].get()};
    Vertex* destination{ m_vertices[destinationId].get() };

    int edgeCheck{ checkForEdge(source, destination) };
    if (edgeCheck > -1) return edgeCheck;       // check for existing edge with the same source and destination
    else if (edgeCheck < -1) return -1;         // invalid parameters

    int edgeId = m_nextEdgeId++;
    auto edge = std::make_unique<Edge>(edgeId, source, destination);
    Edge* edgePtr = edge.get();
    m_edges[edgeId] = std::move(edge);

    // update target vertices - if waypoints full convert to junctions
    if (!source->canAddOut())
    {
        convertToJunction(sourceId);
        source = m_vertices[sourceId].get();
        edgePtr->updateSource(source);
    }
    source->addOut(edgePtr);

    if (!destination->canAddIn())
    {
        convertToJunction(destinationId);
        destination = m_vertices[destinationId].get();
        edgePtr->updateDestination(destination);
    }
    destination->addIn(edgePtr);

    std::cout << "Added edge " << edgeId << " between vertices " << sourceId << ", " << destinationId << '\n';
    return edgeId;
}

std::vector<int> Network::addEdgeEx(int sourceId, int destinationId)
{
    Vertex* source{ m_vertices[sourceId].get() };
    Vertex* destination{ m_vertices[destinationId].get() };

    Vector2 newEdgeTangent{ normalizedTangent(source->pos(), destination->pos()) };

    if (!source || !destination) return {};

    struct EdgePositionPair
    {
        int edgeId{};
        Vector2 pos{};
        EdgePositionPair(int id, Vector2 position) : edgeId{ id }, pos{ position } {}
    };

    std::vector<EdgePositionPair> edgePositions{};
    for (const auto& [eId, edge] : m_edges)
    {
        if (auto intersection{ lineIntersectCap(source->pos(), destination->pos(), edge->src()->pos(), edge->dest()->pos()) })
        {
            std::cout << "Found intersection at " << (*intersection).x << ", " << (*intersection).y << '\n';
            edgePositions.push_back(EdgePositionPair(eId, *intersection));
        }
    }

    std::sort(edgePositions.begin(), edgePositions.end(),
        [source](const EdgePositionPair& a, const EdgePositionPair& b) {
            return Vector2Distance(source->pos(), a.pos) < Vector2Distance(source->pos(), b.pos);
        });

    std::vector<int> addedEdges{};
    if (edgePositions.empty())
    {
        addedEdges.push_back(addEdge(sourceId, destinationId));
        std::cout << "edgePositions empty\n";
        return addedEdges;
    }

    int lastJunction{ -1 };
    for (auto& edgePos : edgePositions)
    {
        int edgeId{ edgePos.edgeId };
        Vector2 intersectionPos{ edgePos.pos };

        int altSource{ m_edges[edgeId]->src()->id() };
        int altDestination{ m_edges[edgeId]->dest()->id() };

        if (!removeEdge(edgeId)) return {};

        int newJunction{ addJunction(intersectionPos) };

        if (lastJunction == -1)
        {
            addedEdges.push_back(addEdge(sourceId, newJunction));
        }
        else
        {
            addedEdges.push_back(addEdge(lastJunction, newJunction));
        }

        addedEdges.push_back(addEdge(altSource, newJunction));
        addedEdges.push_back(addEdge(newJunction, altDestination));

        if (edgeId == edgePositions.back().edgeId)
        {
            addedEdges.push_back(addEdge(newJunction, destinationId));
        }
        lastJunction = newJunction;
    }
    return addedEdges;
}

//std::vector<Edge*> Network::addEdges(Node* srcNode, Node* destNode)
//{
//    // Function prerequisite: sources size must equal destinations size
//    auto srcVertices = srcNode->vertices();
//    auto destVertices = destNode->vertices();
//    
//    if (std::ranges::distance(srcVertices) != std::ranges::distance(destVertices))
//        return {};
//
//    // Function prerequisite: connect only through waypoints
//    for (const auto& source : srcVertices)
//    {
//        if (!source->canAddOut())
//        {
//            return {};
//        }
//    }
//    for (const auto& destination : destVertices)
//    {
//        if (!destination->canAddIn())
//        {
//            return {};
//        }
//    }
//
//    std::vector<Edge*> edgePtrs;
//    const float collinearThreshold = 0.99f;
//    const float validAngleThreshold = 0.0f; // Allows connections within 90 degrees
//
//    // Check if nodes are collinear (on the same straight line)
//    bool collinear = isCollinear(srcNode->pos(), srcNode->tangent(), destNode->pos(), destNode->tangent(), collinearThreshold);
//    
//    if (collinear)
//    {
//        // Create straight edges
//        for (int i = 0; i < std::ranges::distance(srcVertices); ++i)
//        {
//            auto srcVertex = *(srcVertices.begin() + i);
//            auto destVertex = *(destVertices.begin() + i);
//            edgePtrs.push_back(addEdge(srcVertex, destVertex));
//        }
//        return edgePtrs;
//    }
//    else 
//    {
//        // Check if angle is valid for curved connection
//        float dotTangents = dotProduct(srcNode->tangent(), destNode->tangent());
//        if (dotTangents >= validAngleThreshold)
//        {
//            // Calculate appropriate subdivisions based on angle and distance
//            float distance = Vector2Distance(srcNode->pos(), destNode->pos());
//            float angle = acosf(dotTangents); // Angle between tangents
//            
//            // More subdivisions for greater angles or longer distances
//            int subdivisions = static_cast<int>(10 + (distance / 100.0f) + (angle * 10.0f / std::numbers::pi_v<float>));
//            subdivisions = std::max(5, std::min(20, subdivisions));
//            
//            for (int i = 0; i < std::ranges::distance(srcVertices); ++i)
//            {
//                auto srcVertex = *(srcVertices.begin() + i);
//                auto destVertex = *(destVertices.begin() + i);
//                auto edgePtr = addEdge(srcVertex, destVertex, subdivisions, srcNode->tangent(), destNode->tangent());
//                if (edgePtr) edgePtrs.push_back(edgePtr);
//            }
//            return edgePtrs;
//        }
//    }
//    return {};
//}

//Edge* Network::addEdge(Vertex* source, Vertex* destination, int curveSubdiv, Vector2 inTangent, Vector2 outTangent)
//{
//    Vector2 intersection;
//    if (lineIntersect(source->pos(), inTangent, destination->pos(), outTangent, intersection))
//    {
//        QuadBezier curve(source->pos(), intersection, destination->pos(), curveSubdiv);
//        Vertex* currentVertex{ source };
//
//        for (size_t i = 1; i < curve.points().size(); ++i)
//        {
//            const Vector2& point{ curve.points()[i] };
//            if (i == curve.points().size() - 1)
//            {
//                return addEdge(currentVertex, destination);
//            }
//            else
//            {
//                Vertex* newVertex{ addWaypoint(point) };
//                addEdge(currentVertex, newVertex);
//                currentVertex = newVertex;
//            }
//        }
//    }
//    else
//    {
//        addEdge(source, destination);
//    }
//    return nullptr;
//}

int Network::addNode(Vector2 position, int laneCount, Vector2 tangent)
{
	auto node = std::make_unique<Node>(*this, position, laneCount, tangent);
    m_nodes[m_nextNodeId] = std::unique_ptr<Node>(node.release());
    std::cout << "Added vertex " << m_nextNodeId << '\n';
	return m_nextNodeId++;
}

//*
//Segment* Network::addSegment()
//{
//
//}
//*/
//void Network::registerVehicle(Vehicle* vehicle)
//{
//    if (vehicle->edge())
//        m_edgeVehicleMap[vehicle->edge()].push_back(vehicle);
//}
//
//void Network::unregisterVehicle(Vehicle* vehicle)
//{
//    auto& vehicles = m_edgeVehicleMap[vehicle->edge()];
//    vehicles.erase(std::remove(vehicles.begin(), vehicles.end(), vehicle), vehicles.end());
//}

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

int Network::checkForEdge(Vertex* source, Vertex* destination)
{
    if (!source || !destination) return -2; // source or destination null
    if (Waypoint* srcWay = dynamic_cast<Waypoint*>(source))
    {
        if (srcWay->out() && srcWay->out()->dest() == destination)
            return srcWay->out()->id();
        else
            return -1;  // this connection does not exist yet
    }
    else if (Junction* srcJun = dynamic_cast<Junction*>(source))
    {
        for (auto& candidate : srcJun->out())
        {
            if (candidate->dest() == destination)
                return candidate->id();   
        }
        return -1; // this connection does not exist yet
    }
    return -3; // source of unknown type
}

bool Network::removeEdge(int edgeId)
{
    auto it = m_edges.find(edgeId);
    if (it == m_edges.end()) {
        std::cout << "Edge " << edgeId << " not found." << '\n';
        return false; // Edge not found
    }

    Edge* edge = it->second.get();

    // Get the source and destination vertices
    Vertex* source = edge->src();
    Vertex* destination = edge->dest();

    // Remove the edge references from the source and destination vertices
    bool removedFromSource = source->removeOut(edge);
    bool removedFromDest = destination->removeIn(edge);

    // Check whether can trim Junction into Waypoint after removing an edge
    convertToWaypoint(source->id());
    convertToWaypoint(destination->id());

    // Check if removal was successful
    if (!removedFromSource || !removedFromDest) {
        std::cout << "Failed to completely remove edge " << edgeId << " from vertices." << '\n';
        return false;
    }

    // Remove the edge from the map
    m_edges.erase(it);

    std::cout << "Removed edge " << edgeId << " between vertices " << source->id() << " and " << destination->id() << '\n';

    return true;
}

void Network::draw(bool debug)
{
    for (auto& [id, vertex] : m_vertices)
    {
        if (Waypoint* waypoint = dynamic_cast<Waypoint*>(vertex.get()))
        {
            if (waypoint->out())
            {
                Vector2 start = waypoint->pos();
                Vector2 end = waypoint->out()->dest()->pos();
                if (debug) drawArrow(start, end, 2.0f, EDGE_COLOR);
                else DrawLineEx(start, end, LANE_WIDTH, ROAD_COLOR);
            }
            if (debug)
            {
                DrawCircleV(waypoint->pos(), VERTEX_RADIUS, GREEN);
                std::string wid = std::to_string(waypoint->id());
                DrawText(wid.c_str(), static_cast<int>(waypoint->pos().x) + 5, static_cast<int>(waypoint->pos().y) + 5, 20, BLACK);
            }
        }
        if (Junction* junction = dynamic_cast<Junction*>(vertex.get()))
        {
            for (auto& edge : junction->out())
            {
                Vector2 start = junction->pos();
                Vector2 end = edge->dest()->pos();
                if (debug) drawArrow(start, end, 2.0f, EDGE_COLOR);
                else DrawLineEx(start, end, LANE_WIDTH, ROAD_COLOR);
            }
            if (debug)
            {
                DrawCircleV(junction->pos(), VERTEX_RADIUS, DARKGREEN);
                std::string jid = std::to_string(junction->id());
                DrawText(jid.c_str(), static_cast<int>(junction->pos().x) + 5, static_cast<int>(junction->pos().y) + 5, 20, BLACK);
            }
        }
        if (debug)
        {
            //for (auto& node : m_nodes)
            //{
            //    node->drawAxes();
            //}
        }
    }
}

//auto Network::nodes() const {
//	return m_nodes | std::views::transform([](const auto& node) { return node.get(); });
//}

