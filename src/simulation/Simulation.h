#pragma once
#include "Network.h"
#include "Vehicle.h"
#include "VehicleGenerator.h"

class Simulation
{
	float m_totalTime{ 0 };
	float m_simulationSpeed;
	bool m_isPaused{ false };
	
	std::unique_ptr<Network> m_network;
	// std::unique_ptr<Renderer> m_renderer;

public:
	Simulation();
	~Simulation();

	// Deleted copy and move constructors/assignments
	Simulation(const Simulation&) = delete;
	Simulation& operator=(const Simulation&) = delete;
	Simulation(Simulation&&) = delete;
	Simulation& operator=(Simulation&&) = delete;

	void initialize();
	void run();		// runs all the time
	void update();	// updates when unpaused
	void render();
	void shutdown();
	
	void pause();
	void unpause();

	Network* getNetwork() const;
};