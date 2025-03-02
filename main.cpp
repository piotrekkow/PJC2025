#include <vector>
#include <memory>
#include <unordered_map>
#include <iostream>
#include <raylib.h>
#include "utils.h" // Przeci¹¿anie operatorów oraz dodatkowe funkcje do Vector2 zdefiniowanego w raylib.h

constexpr int WINDOW_WIDTH{ 1920 };
constexpr int WINDOW_HEIGHT{ 1080 };

constexpr Color BACKGROUND_COLOR{ RAYWHITE };
constexpr Color EDGE_COLOR{ GREEN };
constexpr Color VERTEX_COLOR{ BLACK };
constexpr Color NORMAL_COLOR{ RED };
constexpr Color TANGENT_COLOR{ BLUE };
constexpr float VERTEX_RADIUS{ 6 };

//! Edge reprezentuje krawêdŸ grafu (w odniesieniu do symulacji drogi jest to pas ruchu)
struct Edge
{
    int m_id;
    int m_destId;                                                             //! Identyfikator wierzcho³ka docelowego

    Edge(int id, int destId) : m_id{ id }, m_destId{ destId } {}
};

enum class InletFor {
    Intersection,
    Segment
};

//! Vertex reprezentuje wierzcho³ek grafu
class Vertex
{
private:
    std::vector<Edge> m_edges;                                                //! Wychodz¹ce krawêdzie
    Vector2 m_pos;                                                            //! Pozycja wierzcho³ka relatywnie do lewego górnego rogu ekranu
    int m_id;
    InletFor m_inletFor;                                                      //! Przechowuje informacjê czy wierzcho³ek jest wlotem do szkrzy¿owania czy do segmentu

public:
    Vertex(int id, Vector2 pos, InletFor inletFor = InletFor::Segment)
        : m_id{ id }
        , m_pos{ pos } 
        , m_inletFor{ inletFor }
    {}

    int getId() const { return m_id; }
    Vector2 getPos() const { return m_pos; }
    std::vector<Edge>& getEdges() { return m_edges; }

    bool isIntersectionInlet() const { return m_inletFor == InletFor::Intersection; }
    bool isSegmentInlet() const { return m_inletFor == InletFor::Segment; }

    void setAsIntersectionInlet() { m_inletFor = InletFor::Intersection; }
    void setAsSegmentInlet() { m_inletFor = InletFor::Segment; }
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

    int getId() const { return m_id; }
    Vector2 getPos() const { return m_pos; }
    std::vector<Vertex*> getVertexPointers() { return m_vertices; }

    Vector2 getTangent() { return m_pos + (tangent() * 30); }                 //! Do debugowania (rysowania stycznej)
    Vector2 getNormal() { return m_pos + (m_normal * 30); }                   //! Do debugowania (rysowania normalnej)

    void fillVertexGroup(int numOfVertices, float laneOffset, int inletCount);

    std::vector<int> getVertexIds()
    {
        std::vector<int> indices;
        indices.reserve(m_vertices.size()); // Lepsza alokacja pamiêci
        for (const auto& vertex : m_vertices)
        {
            indices.push_back(vertex->getId());
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

    Vector2 m_pos;                                                          //! Œrodek skrzy¿owania
    Network& m_network;
    int m_id;
    int m_nextGroupId{ 0 };

public:
    Intersection(Network& network, int id, Vector2 pos);
    VertexGroup* getGroup(int id) { return m_groupMap.count(id) ? m_groupMap[id] : nullptr; }
    void addLeg(Vector2 legTangent, float legOffsetFromCenter, int laneCount, float laneWidth, int inletCount);

    int getId() const { return m_id; }
    Vector2 getPos() const { return m_pos; }

    void drawGroups();                                                      //! Rysowanie grupy na ekranie
    void addAllEdges();

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
            m_vertexMap[srcId]->getEdges().emplace_back(m_nextEdgeId++, destId);
            std::cout << "added edge from " << srcId << " to " << destId << '\n';
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

    void printExistingEdges()       //! debug
    {
        for (auto& [id, vertex] : m_vertexMap)
            for (auto& edge : vertex->getEdges())
                std::cout << id << " -" << edge.m_id << "-> " << m_vertexMap[edge.m_destId]->getId() << '\n';
    }

    void drawNetwork() 
    {
        for (auto& vertex : m_vertices)
        {
            for (auto& edge : vertex->getEdges())
            {
                if (Vertex* dest = getVertex(edge.m_destId))                //! Potwierdzenie, ¿e wierzcho³ek o indeksie edge.m_destId istnieje
                    DrawLineV(vertex->getPos(), getVertex(edge.m_destId)->getPos(), EDGE_COLOR);
            }
            DrawCircleV(vertex->getPos(), VERTEX_RADIUS, VERTEX_COLOR);                        //! Rysujemy ka¿dy wierzcho³ek

            if (vertex->isIntersectionInlet())
                DrawCircleV(vertex->getPos(), VERTEX_RADIUS / 2.0f, BACKGROUND_COLOR);                 //! Gdy jest wlotem do skrzy¿owania rysujemy okr¹g (pozosta³e s¹ wlotami do segmentu)
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
        m_vertexMap[ptr->getId()] = ptr;
        return ptr;
    }

    friend class VertexGroup;
};

VertexGroup::VertexGroup(Network& network, int id, Vector2 pos, Vector2 normal)
    : m_id{ id }
    , m_pos{ pos }
    , m_normal{ normal }
    , m_network{ network }
{}

void VertexGroup::fillVertexGroup(int numOfVertices, float laneWidth, int inletCount)
{
    Vector2 cursorPos{ getStartingPos(numOfVertices, laneWidth) };

    for (int i = 0; i < numOfVertices; ++i)
    {
        Vertex* newVertex{ m_network.addVertex(cursorPos) };
        m_vertices.push_back(newVertex);

        if (i < inletCount)                                             //! Ustawiamy kolejne wierzcho³ki jako wloty skrzy¿owania
            newVertex->setAsIntersectionInlet();
        else
            newVertex->setAsSegmentInlet();

        cursorPos += (tangent() * laneWidth);                           //! utils wykorzystany tutaj do operacji na wektorach (+= oraz *)
    }
}

Intersection::Intersection(Network& network, int id, Vector2 pos)
    : m_network{ network }
    , m_id{ id }
    , m_pos{ pos }
{}

void Intersection::addLeg(Vector2 legTangent, float legOffsetFromCenter, int laneCount, float laneWidth, int inletCount)
{
    VertexGroup* newGroup = addGroup(m_pos + legTangent * legOffsetFromCenter, legTangent);
    newGroup->fillVertexGroup(laneCount, laneWidth, inletCount);
}

void Intersection::addAllEdges()
{
    std::vector<int> inletIds{};
    for (auto& group : m_groups)
    {
        for (auto& vertex : group->getVertexPointers())
        {
            if (vertex->isIntersectionInlet())
            {
                inletIds.push_back(vertex->getId());
            }
        }
    }
    for (auto& group : m_groups)
    {
        for (auto& inletId : inletIds)
        {
            for (auto& destVertex : group->getVertexPointers())
                if (!destVertex->isIntersectionInlet())
                    m_network.addEdge(inletId, destVertex->getId());
        }
    }
}

void Intersection::drawGroups()
{
    for (auto& group : m_groups)
    {
        DrawLineV(group->getPos(), group->getTangent(), TANGENT_COLOR);          //! Wektor styczny
        DrawLineV(group->getPos(), group->getNormal(), NORMAL_COLOR);            //! Wektor normalny
    }
}

int main()
{
    Network network;

    Intersection* intersection1 = network.addIntersection({ 200, 200 });
    Intersection* intersection2 = network.addIntersection({ 800, 200 });
    intersection1->addLeg({ 0, 1 }, 40.0f, 2, 20.0f, 1);
    intersection1->addLeg({ 0, -1 }, 40.0f, 2, 20.0f, 1);
    intersection1->addLeg({ 1, 0 }, 40.0f, 2, 20.0f, 1);
    intersection1->addLeg({ -1, 0 }, 40.0f, 2, 20.0f, 1);
    intersection1->addAllEdges();

    intersection2->addLeg({ 1, 0 }, 20.0f, 3, 20.0f, 1);
    intersection2->addLeg({ -1, 0 }, 20.0f, 2, 20.0f, 1);
    intersection2->addAllEdges();

    network.printExistingEdges(); //! debug

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "PJC2025");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);

        network.drawNetwork();

        EndDrawing();
    }

    CloseWindow();
}