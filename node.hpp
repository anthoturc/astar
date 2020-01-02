#pragma once

#ifndef _NODE_H_

#include <SFML/Graphics.hpp>

class searchnode
{
public:
    sf::RectangleShape * r_;

    searchnode();
    ~searchnode();

    int getRow();
    int getCol();

    void setRow(int row);
    void setCol(int col);

    void setColor(const sf::Color& c);

protected:
    int row_;
    int col_;
};
#endif // _NODE_H_
