#include <vector>
#include <memory>
#include <iostream>
#include <raylib.h>

constexpr int WINDOW_WIDTH{ 1920 };
constexpr int WINDOW_HEIGHT{ 1080 };

constexpr Color BACKGROUND_COLOR{ RAYWHITE };
constexpr Color EDGE_COLOR{ DARKGREEN };
constexpr Color VERTEX_COLOR{ BLACK };
constexpr float VERTEX_RADIUS{ 4 };

class Vertex;

class Edge
{
	Vertex* m_destination;
	int m_id;
	// float speedLimit;

public:
	Edge(int id, Vertex* destination)
		: m_id{ id }
		, m_destination{ destination } 
	{}
	
	int getId() const { return m_id; }
	Vertex* getDestination() const { return m_destination; }
};

class Vertex
{
	std::vector<Edge> m_edges;
	Vector2 m_position;
	int m_id;

public:
	Vertex(int id, Vector2 position)
		: m_id{ id }
		, m_position{position}
	{}

	int getId() const { return m_id; }
	Vector2 getPos() const { return m_position; }
	std::vector<Edge>& getEdges() { return m_edges; }	// may be used in the future for removing edges
	const std::vector<Edge>& getEdges() const { return m_edges; }

	bool isValidDestination(Vertex* destination)	// check if the edge already exists
	{
		const auto& edges{ m_edges };	// create a const reference to the vector to ensure no changes
		for (auto& edge : edges)
		{
			if (edge.getDestination() == destination) { return false; }
		}
		return true;
	}
};

class Graph
{
	std::vector<std::unique_ptr<Vertex>> m_vertices;
	int m_nextVertexId{ 0 }, m_nextEdgeId{ 0 };

public:
	Vertex* addVertex(Vector2 pos)
	{
		auto vertex = std::make_unique<Vertex>(m_nextVertexId, pos);
		Vertex* ptr = vertex.get();
		m_vertices.push_back(std::move(vertex));	// unique_ptr vertex cannot be copied - only moved
		m_nextVertexId++;
		return ptr;
	}

	void addEdge(Vertex* source, Vertex* destination)
	{
		if (source->isValidDestination(destination))
		{
			source->getEdges().emplace_back(m_nextEdgeId, destination);
			m_nextEdgeId++;
		}
		else
		{
			std::cout << "Edge from " << source->getId() << " to " << destination->getId() 
				<< " already exists.\n";
		}
	}

	void draw()
	{
		for (const auto& vertex : m_vertices)
		{
			// draw edges
			for (const auto& edge : vertex->getEdges())
				DrawLineV(vertex->getPos(), edge.getDestination()->getPos(), EDGE_COLOR);
			// draw vertices
			DrawCircleV(vertex->getPos(), VERTEX_RADIUS, VERTEX_COLOR);
		}	
	}
};


int main() 
{
	Graph graph;

	graph.addEdge(graph.addVertex({ 100,100 }), graph.addVertex({ 200,200 }));

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