#include "utils.h"
#include "config.h"
#include "Network.h"
#include "QuadBezier.h"
#include "Segment.h"

int main()
{
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Bezier Curve");
	SetTargetFPS(60);

	// QuadBezier curve({ {10, 10}, {120, 400}, {500, 550} });

	Network network;

	// Node* n1 = network.addNode({ 400, 600 }, 1, { 1, 0 });
	// Node* n2 = network.addNode({ 800, 600 }, 1, { -1, 0 });
	//Node* n3 = network.addNode({ 600, 400 }, 1, { 0, 1 });
	//Node* n4 = network.addNode({ 600, 800 }, 1, { 0, -1 });
	// Node* n5 = network.addNode({ 200, 200 }, 1, { 0, -1 });

	// std::deque<Edge*> path = { network.addEdge(n2->vertices()[0], n1->vertices()[0]), network.addEdge(n1->vertices()[0], n5->vertices()[0]) };
	// std::deque<Edge*> path2 = { network.addEdge(n4->vertices()[0], n3->vertices()[0]) };
	// network.addEdge(network.addWaypoint({ 600,400 }), network.addWaypoint({ 900,100 }), 12, { 1,0 }, { 0,1 });
	
	int w1 = network.addWaypoint({ 200, 200 });
	int w2 = network.addWaypoint({ 400, 200 });
	int w3 = network.addWaypoint({ 200, 1000 });
	int w4 = network.addWaypoint({ 1000, 1000 });
	int w5 = network.addWaypoint({ 700, 300 });
	int w6 = network.addWaypoint({ 100, 800 });

	network.addEdge(w1, w4);
	network.addEdgeEx(w2, w3);
	network.addEdgeEx(w6, w5);
	network.addEdgeEx(w5, w3);

	network.addNode({ 500, 500 }, 4, { 1, 0 });

	//network.addEdge(w3, w1);
	//network.addEdge(w3, w4);

	


	// Vehicle car(network, path, 0);
	// Vehicle car2(network, path2, 0);

	// Segment(network, { 1000,100 }, { 300, 200 }, 2);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		//curve.drawBezier();
		// float delta = GetFrameTime();
		network.draw(true);
		// car.update(delta);
		// car2.update(delta);
		// car.draw();
		// car2.draw();
		EndDrawing();
	}

	CloseWindow();
}