#include "Simulation.h"

Simulation::Simulation()

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
	m_network->draw(true); // temporary - to be replaced with Renderer implementation
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

Network* Simulation::getNetwork() const
{
	return m_network.get();
}
