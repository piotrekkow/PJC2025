#include "Network.h"

int main()
{
	Graph graph;

	graph.addEdge(graph.addVertex({ 100,100 }, true), graph.addVertex({ 200,200 }, true));

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "PJC2025");
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BACKGROUND_COLOR);

		graph.draw();

		EndDrawing();
	}

	CloseWindow();
}