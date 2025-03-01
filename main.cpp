#include <vector>
#include <memory>
#include <unordered_map>
#include <array>
#include <raylib.h>
#include "utils.h" // raylib Vector2 operator overloading

constexpr int WINDOW_WIDTH{ 1920 };
constexpr int WINDOW_HEIGHT{ 1080 };

//! Edge reprezentuje kraw�d� grafu (w odniesieniu do symulacji drogi jest to pas ruchu)
struct Edge {
    int id;                             //! Identyfikator kraw�dzi
    int destId;                         //! Identyfikator wierzcho�ka docelowego

    Edge(int id, int destId) : id(id), destId(destId) {}
};

//! Vertex reprezentuje wierzcho�ek grafu
struct Vertex {
    std::vector<Edge> edges;            //! Wychodz�ce kraw�dzie
    Vector2 pos;                        //! Pozycja wierzcho�ka relatywnie do prawego g�rnego rogu ekranu
    int id;                             //! Identyfikator wierzcho�ka

    Vertex(int id, Vector2 pos) : id(id), pos(pos) {}
};

class Network;

//! VertexGroup reprezentuje struktur� wlotu do skrzy�owania - grup� wierzcho�k�w
//! Definiuje metody budowania grup i rozmieszczenia wierzcho�k�w
//! Wierzcho�ki w grupie le�� na jednej linii
class VertexGroup {
    std::vector<Vertex*> vertices;      //! VertexGroup przechowuje wska�niki z uwagi na potrzeb� posiadania unikalnych wierzcho�k�w w ca�ym grafie
    Vector2 pos;                        //! Pozycja grupy - �rodek ci�ko�ci wierzcho�k�w grupy
    Vector2 normal;                     //! Wektor normalny do linii wierzcho�k�w
    int id;                             //! Identyfikator grupy

public:
    VertexGroup(Network& network, int id, Vector2 pos, Vector2 normal);
    void fillVertexGroup(Network& network, float laneOffset);

    int getId() const { return id; }
    Vector2 getPos() const { return pos; }

    std::vector<int> getVertexIds()
    {
        std::vector<int> indices;
        indices.reserve(vertices.size()); // Lepsza alokacja pami�ci
        for (const auto& vertex : vertices)
        {
            indices.push_back(vertex->id);
        }
        return indices;
    }
    // ... other helper methods ...
};

//! Network reprezentuje najwy�sz� struktur� - ca�y graf
class Network {
    //! Wektory grup i wierzcho�k�w przechowuj� unique_ptr, poniewa� przy zmianie rozmiaru wektora wszystkie istniej�ce zwyk�e wska�niki sta�y by si� wisz�ce
    std::vector<std::unique_ptr<VertexGroup>> groups;               
    std::vector<std::unique_ptr<Vertex>> vertices;
    //! Poni�sze s�owniki pozwalaj� na bezpieczniejsz� prac� ze zbiorami grup i wierzcho�k�w
    //! Bez nich usuni�cie elementu w �rodku wektor�w groups lub vertices spowodowa�oby przesuni�cie wszystkich kolejnych identyfikator�w
    std::unordered_map<int, Vertex*> vertexMap;
    std::unordered_map<int, VertexGroup*> groupMap;

    int nextGroupId{ 0 }, nextVertexId{ 0 }, nextEdgeId{ 0 };

public:

    VertexGroup* addGroup(Vector2 pos, Vector2 normal) {
        auto group = std::make_unique<VertexGroup>(*this, nextGroupId++, pos, normal);
        VertexGroup* ptr = group.get();
        groups.push_back(std::move(group));
        groupMap[ptr->getId()] = ptr;
        return ptr;
    }

    void addEdge(int srcId, int destId) {
        if (vertexMap.count(srcId) && vertexMap.count(destId)) {
            vertexMap[srcId]->edges.emplace_back(nextEdgeId++, destId);
        }
    }

    Vertex* getVertex(int id) { return vertexMap.count(id) ? vertexMap[id] : nullptr; }
    VertexGroup* getGroup(int id) { return groupMap.count(id) ? groupMap[id] : nullptr; }

    void drawNetwork() {
        for (auto& vertex : vertices) {
            for (auto& edge : vertex->edges) {
                if (Vertex* dest = getVertex(edge.destId)) {
                    DrawLineV(vertex->pos, dest->pos, BLACK);
                }
            }
            DrawCircleV(vertex->pos, 4, RED);
        }
        for (auto& group : groups)
            DrawCircleV(group->getPos(), 2, GREEN);
    }

private:
    Vertex* addVertex(Vector2 pos) {
        auto vertex = std::make_unique<Vertex>(nextVertexId++, pos);
        Vertex* ptr = vertex.get();
        vertices.push_back(std::move(vertex));
        vertexMap[ptr->id] = ptr;
        return ptr;
    }

    friend class VertexGroup;
};

// VertexGroup implementation
VertexGroup::VertexGroup(Network& network, int id, Vector2 pos, Vector2 normal) : id(id), pos(pos), normal(normal)
{
    vertices.push_back(network.addVertex(pos));
}

void VertexGroup::fillVertexGroup(Network& network, float laneOffset) {
    Vector2 tangent = { -normal.y, normal.x };

    vertices.push_back(network.addVertex(pos + (tangent * laneOffset)));        //! utils wykorzystany tutaj do operacji na wektorach
    vertices.push_back(network.addVertex(pos - (tangent * laneOffset)));
}

int main()
{
    // Network setup
    Network network;
    
    VertexGroup* group = network.addGroup({ 200, 200 }, { 1, 0 });
    VertexGroup* group2 = network.addGroup({ 800, 200 }, { 1, 0 });
    group->fillVertexGroup(network, 20.0f);
    group2->fillVertexGroup(network, 20.0f);
    network.addEdge(group->getVertexIds()[0], group2->getVertexIds()[0]);
    network.addEdge(group->getVertexIds()[1], group2->getVertexIds()[1]);
    //

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "PJC2025");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        //
        network.drawNetwork();
        //
        EndDrawing();
    }

    CloseWindow();
}