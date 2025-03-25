#include "utils.h"
#include "config.h"
#include "Network.h"
#include "QuadBezier.h"
#include "Segment.h"

#include "Simulation.h"

int main()
{
	Simulation simulation;

	simulation.initialize();
	simulation.run();
}