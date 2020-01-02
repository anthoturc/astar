#include "visualization.hpp"


visualization::visualization(sf::RenderWindow& w) : w_(w)
{
    w_.setFramerateLimit(FRAME_RATE);
}

visualization::~visualization() {}

void
visualization::drawGrid()
{
    for (auto row : grid_) {
        for (auto el : row) {
            w_.draw(*(el->r_));
        }
    }
}

bool
visualization::validNeighbor(int row, int col)
{
    if (row < 0 || row >= (H/BOX_H)) return false;
    if (col < 0 || col >= (W/BOX_W)) return false;
    return true;
}
