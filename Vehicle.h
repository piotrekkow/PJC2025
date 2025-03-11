#pragma once
#include "Edge.h"
#include "Path.h"

enum class VehicleState
{
	
};

class Vehicle
{
	Path m_path;
	float m_speed;
	float m_maxSpeed{ 11.0f };
	float m_acceleration{ 1.5f };
	Vector2 m_size{ 2.0f, 5.0f };
	Edge* m_currentEdge;
	float m_distanceAlongEdge{ 0 };

public:
	Vehicle(Path path, float initSpeed);
	void update(float deltaTime);

private:
	void draw() const;
};

