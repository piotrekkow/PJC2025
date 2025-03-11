#include <raylib.h>
#include <vector>
#include <memory> // unique_ptr
#include <utility> // pair
#include <array>

#include "utils.h"
#include "config.h"
#include "Network.h"

class QuadBezier
{
	static constexpr int m_subdivisions{ 16 };
	std::array<Vector2, 3> m_controlPoints{}; // control points
	std::array<Vector2, m_subdivisions + 1> m_points;

	Vector2 getCurvePoint(float t) const
	{
		float u = 1.0f - t;
		float tt = t * t;
		float uu = u * u;
		return {
			uu * m_controlPoints[0].x + 2 * t * u * m_controlPoints[1].x + tt * m_controlPoints[2].x,
			uu * m_controlPoints[0].y + 2 * t * u * m_controlPoints[1].y + tt * m_controlPoints[2].y
		};
	}

	void setOfPoints()
	{
		for (int i = 0; i <= m_subdivisions; ++i)
		{
			float t = static_cast<float>(i) / m_subdivisions;
			m_points[i] = getCurvePoint(t);
		}
	}

public:
	QuadBezier(std::array<Vector2, 3> points) : m_controlPoints{ points }
	{
		setOfPoints();
	}

	QuadBezier(Vector2 point1, Vector2 point2, Vector2 point3)
		: QuadBezier(std::array<Vector2, 3>{ point1, point2, point3 }) {
	}


	void drawBezier() const
	{
		for (int i = 0; i < m_subdivisions; ++i)
		{
			DrawLineV(m_points[i], m_points[i + 1], BLACK);
		}
	}
};

class Segment
{
	std::pair<Node*, Node*> m_ends;
	Vector2 tangent;
};

int main()
{
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Bezier Curve");
	SetTargetFPS(60);

	// QuadBezier curve({ {10, 10}, {120, 400}, {500, 550} });

	Network network;

	Node* n1 = network.addNode({ 400,600 }, 1, { 1, 0 });
	Node* n2 = network.addNode({ 800,600 }, 1, { -1, 0 });
	Node* n3 = network.addNode({ 600,400 }, 1, { 0, 1 });
	Node* n4 = network.addNode({ 600,800 }, 1, { 0, -1 });

	network.addEdge(n2->vertices()[0], n1->vertices()[0]);
	network.addEdge(n4->vertices()[0], n3->vertices()[0]);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		//curve.drawBezier();
		network.draw(true);
		EndDrawing();
	}

	CloseWindow();
}