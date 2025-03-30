#include "Network.h"
#include "config.h"
#include "utils.h" // Vector2 operator*, operator+

Node* Network::addNode(Vector2 position, Vector2 tangent, int size)
{
	std::unique_ptr<Node> node = std::make_unique<Node>(position, tangent, size);
	m_nodes.emplace_back(std::move(node));
	return m_nodes.back().get();
}

//std::vector<Node*> Network::addRoad(Node* previousNode, Vector2 position, Vector2 tangent, int size)
//{
//	return std::vector<Node*>();
//}
//
//std::vector<Node*> Network::addRoad(Node* source, Node* destination)
//{
//	return std::vector<Node*>();
//}
//
//std::vector<Node*> Network::addRoad(Vector2 position, Vector2 tangent, int size, Node* nextNode)
//{
//	return std::vector<Node*>();
//}

void Network::draw(bool debug)
{
    for (auto& node : m_nodes)
    {
        if (node->getOut())
        {
            for (auto& edge : node->getOut()->getEdges())
            {
                edge->draw();
            }
        }

        for (auto& vertex : node->getVertices())
        {
            DrawCircleV(vertex->getPos(), VERTEX_RADIUS, VERTEX_COLOR);
        }
        DrawLineV(node->getPos(), node->getPos() + node->getTangent() * DEBUG_AXES_LENGTH, TANGENT_COLOR);
    }
    if (debug) {}
}