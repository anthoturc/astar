#include "astarnode.hpp"
#include <SFML/Graphics.hpp>

astarnode::astarnode(sf::RectangleShape * r)
    : gcost_(INT_MAX),
    hcost_(INT_MAX), isobstacle_(false),
    predecessor_(nullptr), r_(r)
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

int
astarnode::getCol()
{
    return col_;
}

int
astarnode::getRow()
{
    return row_;
}

void
astarnode::setCol(int col)
{
    col_ = col;
}

void
astarnode::setRow(int row)
{
    row_ = row;
}
