#include "astarnode.hpp"
#include <SFML/Graphics.hpp>

astarnode::astarnode(sf::RectangleShape * r)
    : visited_(false), gcost_(INT_MAX), hcost_(INT_MAX), predecessor_(nullptr), r_(r)
{}



int
astarnode::getFCost()
{
    return gcost_ + hcost_;
}

void
astarnode::setGCost(int gcost)
{
    gcost_ = gcost;
}

int
astarnode::getGCost()
{
    return gcost_;
}

void
astarnode::setHCost(int hcost)
{
    hcost_ = hcost;
}

int
astarnode::getHCost()
{
    return hcost_;
}

astarnode *
astarnode::getPredecessor()
{
    return predecessor_;
}

void
astarnode::setPredecessor(astarnode * predecessor)
{
    predecessor_ = predecessor;
}
