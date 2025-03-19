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

	Node* n1 = network.addNode({ 400, 600 }, 1, { 1, 0 });
	Node* n2 = network.addNode({ 800, 600 }, 1, { -1, 0 });
	//Node* n3 = network.addNode({ 600, 400 }, 1, { 0, 1 });
	//Node* n4 = network.addNode({ 600, 800 }, 1, { 0, -1 });
	Node* n5 = network.addNode({ 200, 200 }, 1, { 0, -1 });

	std::deque<Edge*> path = { network.addEdge(n2->vertices()[0], n1->vertices()[0]), network.addEdge(n1->vertices()[0], n5->vertices()[0]) };
	// std::deque<Edge*> path2 = { network.addEdge(n4->vertices()[0], n3->vertices()[0]) };
	network.addEdge(network.addWaypoint({ 600,400 }), network.addWaypoint({ 900,100 }), 12, { 1,0 }, { 0,1 });
	Waypoint* w1 = network.addWaypoint({ 800, 200 });
	network.addEdge(n1->vertices()[0], w1);
	Vehicle car(network, path, 0);
	// Vehicle car2(network, path2, 0);

	Segment(network, { 1000,100 }, { 300, 200 }, 2);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		//curve.drawBezier();
		float delta = GetFrameTime();
		network.draw(true);
		car.update(delta);
		// car2.update(delta);
		car.draw();
		// car2.draw();
		EndDrawing();
	}

	CloseWindow();
}