#include <vector>
#include <memory>
#include <unordered_map>
#include <array>
#include <raylib.h>
#include "utils.h" // Przeci¹¿anie operatorów oraz dodatkowe funkcje do Vector2 zdefiniowanego w raylib.h

constexpr int WINDOW_WIDTH{ 1920 };
constexpr int WINDOW_HEIGHT{ 1080 };

//! Edge reprezentuje krawêdŸ grafu (w odniesieniu do symulacji drogi jest to pas ruchu)
struct Edge
{
    int m_id;
    int m_destId;                                                             //! Identyfikator wierzcho³ka docelowego

    Edge(int id, int destId) : m_id{ id }, m_destId{ destId } {}
};

//! Vertex reprezentuje wierzcho³ek grafu
struct Vertex
{
    std::vector<Edge> m_edges;                                                //! Wychodz¹ce krawêdzie
    Vector2 m_pos;                                                            //! Pozycja wierzcho³ka relatywnie do lewego górnego rogu ekranu
    int m_id;

    Vertex(int id, Vector2 pos) : m_id{ id }, m_pos{ pos } {}
};


class Network;                                                                //! Deklaracja do przodu, wykorzystywane w VertexGroup i Intersection

//! VertexGroup reprezentuje strukturê wlotu do skrzy¿owania - grupê wierzcho³ków
//! Definiuje metody budowania grup i rozmieszczenia wierzcho³ków
//! Wierzcho³ki w grupie le¿¹ na jednej linii
class VertexGroup
{
    std::vector<Vertex*> m_vertices;                                          //! VertexGroup przechowuje wskaŸniki z uwagi na potrzebê posiadania unikalnych wierzcho³ków w ca³ym grafie
    Vector2 m_pos;                                                            //! Pozycja grupy - œrodek ciê¿koœci wierzcho³ków grupy
    Vector2 m_normal;                                                         //! Wektor normalny do linii wierzcho³ków
    Network& m_network;
    int m_id;

public:
    VertexGroup(Network& network, int id, Vector2 pos, Vector2 normal);
    void fillVertexGroup(int numOfVertices, float laneOffset);

    int getId() const { return m_id; }
    Vector2 getPos() const { return m_pos; }

    Vector2 getTangent() { return m_pos + (tangent() * 30); }                 //! Do debugowania (rysowania stycznej)
    Vector2 getNormal() { return m_pos + (m_normal * 30); }                   //! Do debugowania (rysowania normalnej)

    std::vector<int> getVertexIds()
    {
        std::vector<int> indices;
        indices.reserve(m_vertices.size()); // Lepsza alokacja pamiêci
        for (const auto& vertex : m_vertices)
        {
            indices.push_back(vertex->m_id);
        }
        return indices;
    }
    
private:
    Vector2 tangent() { return { -m_normal.y, m_normal.x }; }                 //! Wektor styczny do linii wierzcho³ków
    Vector2 getStartingPos(int numOfVertices, float laneOffset)
    {
        if (numOfVertices < 2 || laneOffset == 0) return m_pos;               //! Je¿eli zero lub jeden wierzcho³ek lub szerokoœæ pasa zero
        float width{ (numOfVertices - 1) * laneOffset };
        return m_pos - (tangent() * (width / 2));                             //! Pozycja pocz¹tkowa jest oddalona o po³owê szerokoœci grupy od jej œrodka ciê¿koœci wzd³u¿ stycznej
    }
};

//! Intersection reprezentuje skrzy¿owanie
//! Skrzy¿owanie sk³ada siê z grup wierzcho³ków oraz krawêdzi
class Intersection
{
    //! Wektory grup przechowuj¹ unique_ptr
    //! poniewa¿ przy zmianie rozmiaru wektora wszystkie istniej¹ce zwyk³e wskaŸniki sta³y by siê wisz¹ce
    std::vector<std::unique_ptr<VertexGroup>> m_groups;
    //! Poni¿szy s³ownik pozwalaja na bezpieczniejsz¹ pracê ze zbiorami grup i wierzcho³ków
    //! Bez niego usuniêcie elementu w œrodku wektora groups spowodowa³oby przesuniêcie wszystkich kolejnych identyfikatorów
    std::unordered_map<int, VertexGroup*> m_groupMap;
    Vector2 m_pos;
    Network& m_network;
    int m_id;
    int m_nextGroupId{ 0 };

public:
    Intersection(Network& network, int id, Vector2 pos);
    VertexGroup* getGroup(int id) { return m_groupMap.count(id) ? m_groupMap[id] : nullptr; }
    void addLeg(Vector2 legTangent, float legOffsetFromCenter, int laneCount, float laneWidth);

    int getId() const { return m_id; }
    Vector2 getPos() const { return m_pos; }

    void drawGroups()                                                       //! Rysowanie grupy na ekranie
    {
        for (auto& group : m_groups)
        {
            DrawCircleV(group->getPos(), 2, GREEN);                         //! Œrodek ciê¿koœci
            DrawLineV(group->getPos(), group->getTangent(), BLUE);          //! Wektor styczny
            DrawLineV(group->getPos(), group->getNormal(), RED);            //! Wektor normalny
        }
    }

private:
    VertexGroup* addGroup(Vector2 position, Vector2 normal)
    {
        //! Sprawdzenie czy grupa z wskazanymi parametrami ju¿ istnieje
        for (const auto& [id, group] : m_groupMap) {
            if (group->getPos() == position && group->getNormal() == normal) {      //! utils wykorzystany tutaj do operacji na wektorach
                return group;
            }
        }

        auto group = std::make_unique<VertexGroup>(m_network, m_nextGroupId, position, normal);
        VertexGroup* ptr = group.get();
        m_groups.push_back(std::move(group));
        m_groupMap[m_nextGroupId] = ptr;
        m_nextGroupId++;

        return ptr;
    }
};


//! Network reprezentuje najwy¿sz¹ strukturê - ca³y graf
class Network
{
    //! Wektory skrzy¿owañ i wierzcho³ków przechowuj¹ unique_ptr
    //! a) poniewa¿ przy zmianie rozmiaru wektora wszystkie istniej¹ce zwyk³e wskaŸniki sta³y by siê wisz¹ce
    //! b) s¹ unikalne w skali grafu
    std::vector<std::unique_ptr<Vertex>> m_vertices;
    std::vector<std::unique_ptr<Intersection>> m_intersections;
    //! Poni¿sze s³owniki pozwalaj¹ na bezpieczniejsz¹ pracê ze zbiorami grup i wierzcho³ków
    //! Bez nich usuniêcie elementu w œrodku wektorów intersections lub vertices spowodowa³oby przesuniêcie wszystkich kolejnych identyfikatorów
    std::unordered_map<int, Vertex*> m_vertexMap;
    std::unordered_map<int, Intersection*> m_intersectionMap;

    int m_nextIntersectionId{ 0 }, m_nextVertexId{ 0 }, m_nextEdgeId{ 0 };

public:
    //! Dodajemy krawêdŸ bezpoœrednio przez m_vertexMap, bo jest szybsze od przejœcia ca³ego for loopa m_vertices
    void addEdge(int srcId, int destId) 
    {
        if (m_vertexMap.count(srcId) && m_vertexMap.count(destId))
        {
            m_vertexMap[srcId]->m_edges.emplace_back(m_nextEdgeId++, destId);
        }
    }

    Vertex* getVertex(int id) { return m_vertexMap.count(id) ? m_vertexMap[id] : nullptr; }
    Intersection* getIntersection(int id) { return m_intersectionMap.count(id) ? m_intersectionMap[id] : nullptr; }

    Intersection* addIntersection(Vector2 position)
    {
        //! Sprawdzenie czy grupa z wskazanymi parametrami ju¿ istnieje
        for (const auto& [id, intersection] : m_intersectionMap) {
            if (intersection->getPos() == position) {      //! utils wykorzystany tutaj do operacji na wektorach
                return intersection;
            }
        }

        auto intersection = std::make_unique<Intersection>(*this, m_nextIntersectionId, position);
        Intersection* ptr = intersection.get();
        m_intersections.push_back(std::move(intersection));
        m_intersectionMap[ptr->getId()] = ptr;
        m_nextIntersectionId++;

        return ptr;
    }

    void drawNetwork() 
    {
        for (auto& vertex : m_vertices)
        {
            for (auto& edge : vertex->m_edges)
            {
                if (Vertex* dest = getVertex(edge.m_destId))
                {
                    DrawLineV(vertex->m_pos, dest->m_pos, BEIGE);
                }
            }
            DrawCircleV(vertex->m_pos, 4, BROWN);
        }
        for (auto& intersection : m_intersections)
        {
            intersection->drawGroups();
        }
    }

private:
    Vertex* addVertex(Vector2 pos) 
    {
        auto vertex = std::make_unique<Vertex>(m_nextVertexId++, pos);
        Vertex* ptr = vertex.get();
        m_vertices.push_back(std::move(vertex));
        m_vertexMap[ptr->m_id] = ptr;
        return ptr;
    }

    friend class VertexGroup;
    friend class Intersection;
};

VertexGroup::VertexGroup(Network& network, int id, Vector2 pos, Vector2 normal)
    : m_id{ id }
    , m_pos{ pos }
    , m_normal{ normal }
    , m_network{ network }
{}

void VertexGroup::fillVertexGroup(int numOfVertices, float laneOffset)
{
    Vector2 cursorPos{ getStartingPos(numOfVertices, laneOffset) };

    for (int i = 0; i < numOfVertices; ++i)
    {
        m_vertices.push_back(m_network.addVertex(cursorPos));  //! utils wykorzystany tutaj do operacji na wektorach
        cursorPos += (tangent() * laneOffset);
    }
}

Intersection::Intersection(Network& network, int id, Vector2 pos)
    : m_network{ network }
    , m_id{ id }
    , m_pos{ pos }
{}

void Intersection::addLeg(Vector2 legTangent, float legOffsetFromCenter, int laneCount, float laneWidth)
{
    VertexGroup* group = addGroup(m_pos + legTangent * legOffsetFromCenter, legTangent);
    group->fillVertexGroup(laneCount, laneWidth);
}

int main()
{
    Network network;
    
    Intersection* intersection1 = network.addIntersection({ 200, 200 });
    // Intersection* intersection2 = network.addIntersection({ 800, 200 });
    intersection1->addLeg({ 0, 1 }, 40.0f, 2, 20.0f);
    intersection1->addLeg({ 0, -1 }, 40.0f, 2, 20.0f);
    network.addEdge(intersection1->getGroup(0)->getVertexIds()[0], intersection1->getGroup(1)->getVertexIds()[1]);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "PJC2025");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        network.drawNetwork();

        EndDrawing();
    }

    CloseWindow();
}