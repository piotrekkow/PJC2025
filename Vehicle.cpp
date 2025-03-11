#include "Vehicle.h"

Vehicle::Vehicle(Path path, float initSpeed)
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
