#pragma once
#include "Edge.h"
#include "Path.h"

class Vehicle
{
    // Existing members
    Path m_path;
    float m_speed;
    float m_maxSpeed{ 11.0f };
    float m_acceleration{ 1.5f };
    float m_deceleration{ 3.0f }; // Added for braking
    Vector2 m_size{ 2.0f, 5.0f };
    Edge* m_currentEdge;
    float m_distanceAlongEdge{ 0 };
    Network& m_network; // Reference to network (add this)

    // New members for yielding behavior
    float m_lookAheadDistance{ 50.0f }; // How far ahead to look for intersections
    bool m_yielding{ false }; // Whether vehicle is currently yielding

    // VehicleState enum extended
    enum class VehicleState
    {
        MOVING,
        YIELDING,
        STOPPED
    };

    VehicleState m_state{ VehicleState::MOVING };

public:
    Vehicle(Path path, float initSpeed, Network& network);
    void update(float deltaTime);
    
    // Getters
    Edge* getCurrentEdge() const { return m_currentEdge; }
    float getDistanceAlongEdge() const { return m_distanceAlongEdge; }
    float getLength() const { return m_size.y; } // Length in direction of travel
    Vector2 getPosition() const;

private:
    void draw() const;
    void followPath(float deltaTime);
    bool checkForYielding();
    std::vector<std::pair<Vertex*, float>> getUpcomingVertices() const;
};
