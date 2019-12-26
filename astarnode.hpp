
#pragma once

#ifndef _A_STAR_NODE_
#define _A_STAR_NODE_

#include <SFML/Graphics.hpp>

class astarnode
{
private:
    bool visited_;
    int gcost_;
    int hcost_;

    astarnode * predecessor_;

public:
    sf::RectangleShape * r_;


    astarnode(sf::RectangleShape *r);
    ~astarnode();

    bool seen();

    int getFCost();
    int getGCost();
    int getHCost();

    void setGCost(int gcost);
    void setHCost(int hcost);

    astarnode * getPredecessor();
    void setPredecessor(astarnode * predecessor);
};

#endif // _A_STAR_NODE_
