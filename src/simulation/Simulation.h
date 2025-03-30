#pragma once
#include "Network.h"

class Simulation
{
	/** Time since the begining of the simulation */
	float m_totalTime{ 0 };

	/** Simulation speed 1.0f is 1s in simulation = 1s in irl */
	float m_simulationSpeed;
	bool m_isPaused;
	
	/** Network stores all the infrastructure */
	std::unique_ptr<Network> m_network;
	// std::unique_ptr<Renderer> m_renderer;

public:
	Simulation(bool isPaused = false, float simulationSpeed = 1.0f);
	~Simulation();

	// Deleted copy and move constructors/assignments
	Simulation(const Simulation&) = delete;
	Simulation& operator=(const Simulation&) = delete;
	Simulation(Simulation&&) = delete;
	Simulation& operator=(Simulation&&) = delete;

	void initialize();

	/** Runs all the time */
	void run();

	/** Updates when unpaused */
	void update();
	void render();
	void shutdown();
	
	void pause();
	void unpause();
};