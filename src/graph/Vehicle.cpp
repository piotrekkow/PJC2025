#include "Vehicle.h"

Vehicle::Vehicle(Network& network, std::deque<Edge*> path, float initialSpeed)
    : m_network{ network }
    , m_path{ path }
    , m_speed{ initialSpeed }
{
    if (!path.empty())
    {
        m_currentEdge = m_path.front();
    }
}

void Vehicle::update(float deltaTime)
{
    m_speed += m_acceleration * deltaTime;
    
    if (m_speed > m_maxSpeed)
        m_speed = m_maxSpeed;

    m_distanceAlongEdge += m_speed * deltaTime;
    if (m_distanceAlongEdge >= m_currentEdge->length())
    {
        Edge* oldEdge = m_currentEdge;
        m_path.pop_front();

        if (!m_path.empty())
        {
            m_currentEdge = m_path.front();
            m_distanceAlongEdge -= oldEdge->length();
        }
        else
        {
            
        }
    }
}

void Vehicle::draw()
{
    DrawRectangleV(position(), m_size, PURPLE);
}

Vector2 Vehicle::position()
{
    Vector2 sourcePosition = m_currentEdge->src()->pos();
    return sourcePosition + m_currentEdge->tangent() * m_distanceAlongEdge;
}




/*
#include "Vehicle.h"

Vehicle::Vehicle(Network& network, Path path, float initSpeed)
	: m_path{ path }
	, m_currentEdge{ path.edges()[0] }
{
	if (initSpeed > m_maxSpeed)
	{
		m_speed = initSpeed;
	}
	else
	{
		m_speed = m_maxSpeed;
	}
}

void Vehicle::update(float deltaTime)
{
	followPath()
}

bool Vehicle::checkForYielding()
{
    // Get upcoming vertices within look-ahead distance
    auto upcomingVertices = getUpcomingVertices();
    
    for (const auto& [vertex, distance] : upcomingVertices) {
        // Don't check vertices too far away
        if (distance > m_lookAheadDistance) continue;
        
        // Check each incoming edge at this vertex (except those in our path)
        for (Edge* incomingEdge : vertex->getInEdges()) {
            // Skip edges that are part of our path
            bool edgeInPath = false;
            for (Edge* pathEdge : m_path.edges()) {
                if (pathEdge == incomingEdge) {
                    edgeInPath = true;
                    break;
                }
            }
            
            if (edgeInPath) continue;
            
            // Get vehicles on this edge
            auto vehiclesOnEdge = m_network.getVehiclesOnEdge(incomingEdge);
            
            for (Vehicle* otherVehicle : vehiclesOnEdge) {
                // Calculate other vehicle's distance to intersection
                float otherDistance = incomingEdge->getLength() - otherVehicle->getDistanceAlongEdge();
                
                // If other vehicle is close to intersection and has priority
                if (otherDistance < 30.0f && shouldGiveWayTo(otherVehicle, vertex, distance, otherDistance)) {
                    return true; // We should yield
                }
            }
        }
    }
    
    return false;
}

std::vector<std::pair<Vertex*, float>> Vehicle::getUpcomingVertices() const
{
    std::vector<std::pair<Vertex*, float>> vertices;
    
    // First vertex is at the end of current edge
    float distToNextVertex = m_currentEdge->getLength() - m_distanceAlongEdge;
    vertices.push_back({m_currentEdge->dest(), distToNextVertex});
    
    // Find current edge in path
    size_t currentEdgeIndex = 0;
    for (size_t i = 0; i < m_path.edges().size(); i++) {
        if (m_path.edges()[i] == m_currentEdge) {
            currentEdgeIndex = i;
            break;
        }
    }
    
    // Continue adding vertices from future edges
    float accumulatedDistance = distToNextVertex;
    for (size_t i = currentEdgeIndex + 1; i < m_path.edges().size(); i++) {
        Edge* edge = m_path.edges()[i];
        accumulatedDistance += edge->getLength();
        
        // Stop if we've exceeded look-ahead distance
        if (accumulatedDistance > m_lookAheadDistance) break;
        
        vertices.push_back({edge->dest(), accumulatedDistance});
    }
    
    return vertices;
}

bool Vehicle::shouldGiveWayTo(Vehicle* other, Vertex* intersection, 
                             float myDistance, float theirDistance)
{
    // Simple priority rules:
    
    // Rule 1: Priority to the right (depends on edge angle)
    // This requires calculating the angle between edges at the intersection
    Vector2 myDirection = Vector2Normalize(m_currentEdge->dest()->pos() - m_currentEdge->source()->pos());
    Vector2 theirDirection = Vector2Normalize(other->getCurrentEdge()->dest()->pos() - 
                                           other->getCurrentEdge()->source()->pos());
    
    // Calculate angle between directions (simplified)
    float crossProduct = myDirection.x * theirDirection.y - myDirection.y * theirDirection.x;
    if (crossProduct < 0) {
        // Other vehicle is coming from right
        return true;
    }
    
    // Rule 2: First to arrive has priority (based on time to intersection)
    float myTimeToIntersection = myDistance / m_speed;
    float theirTimeToIntersection = theirDistance / other->m_speed;
    
    if (theirTimeToIntersection < myTimeToIntersection - 0.5f) {
        // They'll arrive significantly earlier
        return true;
    }
    
    // Could add more rules here (major/minor roads, traffic signals, etc.)
    
    return false;
}

void Vehicle::update(float deltaTime)
{
    // Check if we need to yield based on upcoming intersections
    bool needToYield = checkForYielding();
    
    if (needToYield) {
        // Transition to yielding state
        m_state = VehicleState::YIELDING;
        
        // Apply braking
        m_speed = std::max(0.0f, m_speed - m_deceleration * deltaTime);
        
        // If nearly stopped, come to a complete stop
        if (m_speed < 0.5f) {
            m_speed = 0.0f;
            m_state = VehicleState::STOPPED;
        }
    } else {
        // Transition to moving state
        if (m_state != VehicleState::MOVING) {
            m_state = VehicleState::MOVING;
        }
        
        // Resume normal speed
        m_speed = std::min(m_maxSpeed, m_speed + m_acceleration * deltaTime);
    }
    
    // Update position along path
    followPath(deltaTime);
}

void Vehicle::followPath(float deltaTime)
{
    // Calculate movement for this frame
    float movement = m_speed * deltaTime;
    
    // Move along current edge
    m_distanceAlongEdge += movement;
    
    // Check if vehicle has reached the end of the edge
    if (m_distanceAlongEdge >= m_currentEdge->getLength()) {
        // Find the index of the current edge in the path
        size_t currentEdgeIndex = 0;
        for (size_t i = 0; i < m_path.edges().size(); i++) {
            if (m_path.edges()[i] == m_currentEdge) {
                currentEdgeIndex = i;
                break;
            }
        }
        
        // Check if this is the last edge in the path
        if (currentEdgeIndex < m_path.edges().size() - 1) {
            // Move to the next edge
            Edge* oldEdge = m_currentEdge;
            m_currentEdge = m_path.edges()[currentEdgeIndex + 1];
            m_distanceAlongEdge = m_distanceAlongEdge - oldEdge->getLength();
            
            // Update network tracking
            m_network.updateVehicleEdge(this, oldEdge, m_currentEdge);
        } else {
            // Reached end of path
            // Handle vehicle reaching destination (remove from simulation, etc.)
        }
    }
}

void Vehicle::draw() const
{
    // Draw vehicle body
    Vector2 position = getPosition();
    DrawRectanglePro(
        Rectangle{position.x, position.y, m_size.x, m_size.y},
        Vector2{m_size.x / 2, m_size.y / 2},
        0.0f, // Rotation would be calculated based on edge direction
        BLUE
    );
    
    // Debug visualization of yielding behavior
    if (m_state == VehicleState::YIELDING || m_state == VehicleState::STOPPED) {
        // Indicate yielding with a red circle
        DrawCircleV(Vector2{position.x, position.y - 10.0f}, 3.0f, RED);
    }
    
    // Visualize look-ahead area if in debug mode
    #ifdef DEBUG_MODE
    // Draw current edge
    DrawLineV(m_currentEdge->source()->pos(), m_currentEdge->dest()->pos(), YELLOW);
    
    // Draw upcoming vertices being checked
    auto upcomingVertices = getUpcomingVertices();
    for (const auto& [vertex, distance] : upcomingVertices) {
        if (distance <= m_lookAheadDistance) {
            // Draw vertex and its connecting edges
            DrawCircleV(vertex->pos(), 5.0f, ORANGE);
            
            // Draw edges being checked for vehicles
            for (Edge* edge : vertex->getInEdges()) {
                // Skip edges in our path
                bool edgeInPath = false;
                for (Edge* pathEdge : m_path.edges()) {
                    if (pathEdge == edge) {
                        edgeInPath = true;
                        break;
                    }
                }
                
                if (!edgeInPath) {
                    // This is a crossing edge we're checking
                    DrawLineV(edge->source()->pos(), edge->dest()->pos(), PINK);
                }
            }
        }
    }
    #endif
}

Vector2 Vehicle::getPosition() const
{
    // Interpolate position along edge
    float t = m_distanceAlongEdge / m_currentEdge->getLength();
    Vector2 source = m_currentEdge->source()->pos();
    Vector2 dest = m_currentEdge->dest()->pos();
    
    return {
        source.x + t * (dest.x - source.x),
        source.y + t * (dest.y - source.y)
    };
}
*/