#include "node.hpp"

searchnode::searchnode() : r_(nullptr) {}

searchnode::~searchnode() {}

int
searchnode::getCol()
{
    return col_;
}

int
searchnode::getRow()
{
    return row_;
}

void
searchnode::setCol(int col)
{
    col_ = col;
}

void
searchnode::setRow(int row)
{
    row_ = row;
}

void
searchnode::setColor(const sf::Color& c)
{
    r_->setFillColor(c);
}
