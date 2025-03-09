#include "Network.h"
#include "Angle.h"

int main()
{
	Graph graph;
	Network network(graph);

	Intersection* is = network.addIntersection({ 400,400 });
	[[maybe_unused]] Leg* leg = is->addLeg({ 0, 20 }, 4, 2);

	network.addRoad({ 600, 200 }, { 300, 800 });
	network.addRoad(is, { 200,200 });
	
	leg->getAngle().print();

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "PJC2025");
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BACKGROUND_COLOR);
		
		network.draw(true);
		
		EndDrawing();
	}

	CloseWindow();
}