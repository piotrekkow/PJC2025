#pragma once
#include "Network.h"
#include "Vehicle.h"
#include "VehicleGenerator.h"

class Simulation
{
	Network& m_network;
	std::vector<Vehicle> m_vehicles;
	std::vector<VehicleGenerator> m_generators;

public:
	void update(float deltaTime);
	void draw() const;
};