#pragma once
#include "Vertex.h"

class VehicleGenerator
{
	Vertex* m_source;
	float m_vehiclesPerHour;

public:
	VehicleGenerator(Vertex* source, float vehiclesPerHour);
};

