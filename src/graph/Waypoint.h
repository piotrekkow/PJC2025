#pragma once
#include "Vertex.h"
class Waypoint :
    public Vertex
{
    Edge* m_in;
    Edge* m_out;

public:
    Edge* in() const;
    Edge* out() const;
};
