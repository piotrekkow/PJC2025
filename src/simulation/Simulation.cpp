#include "Simulation.h"
#include "config.h"
#include <iostream>

Simulation::Simulation(bool isPaused, float simulationSpeed)
	: m_simulationSpeed{ simulationSpeed }
	, m_isPaused{ isPaused }
{}

Simulation::~Simulation()
{
	shutdown();
}

void Simulation::initialize()
{
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
	SetTargetFPS(60);
	m_network = std::make_unique<Network>();

	Node* node1 = m_network->addNode({ 100, 100 }, { 1, 0 }, 3);
	Node* node2 = m_network->addNode({ 300, 150 }, { 0.8f, 0.2f }, 3);

	node1->addOutSegment(node2);
}

void Simulation::run()
{
	while (!WindowShouldClose())
	{
		if (!m_isPaused)
		{
			update();
		}
		render();
	}
}

void Simulation::update()
{
	float deltaTime{ GetFrameTime() * m_simulationSpeed };
	m_totalTime += deltaTime;
	// update vehicles here in the future
}

void Simulation::render()
{
	BeginDrawing();
	ClearBackground(BACKGROUND_COLOR);

	m_network->draw(true); // temporary - to be replaced with Renderer implementation

	EndDrawing();
}

void Simulation::shutdown()
{
	// possible clean-up

	CloseWindow();
}

void Simulation::pause()
{
	m_isPaused = true;
}

void Simulation::unpause()
{
	m_isPaused = false;
}

