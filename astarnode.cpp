#include "astarnode.hpp"
#include <SFML/Graphics.hpp>

astarnode::astarnode()
    : gcost_(INT_MAX), hcost_(INT_MAX), isobstacle_(false), predecessor_(nullptr)
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

void
astarnode::setObstacle(bool status)
{
    isobstacle_ = status;
}

bool
astarnode::isObstacle()
{
    return isobstacle_;
}
