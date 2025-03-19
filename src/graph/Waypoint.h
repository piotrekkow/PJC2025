#pragma once
#include "Vertex.h"
class Waypoint :
    public Vertex
{
    Edge* m_in{ nullptr };
    Edge* m_out{ nullptr };

public:
	Waypoint(Vector2 position);
    Edge* in() const;
    Edge* out() const;
    bool canAddIn() override;
    bool canAddOut() override;
    bool addIn(Edge* inEdge) override;
    bool addOut(Edge* outEdge) override;
};
