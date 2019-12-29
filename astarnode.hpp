
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
    bool isobstacle_;
    int row_;
    int col_;

    astarnode * predecessor_;

public:
    sf::RectangleShape * r_;


    astarnode(sf::RectangleShape *r);
    ~astarnode();

    bool seen();

    int getFCost();

    int getGCost();
    int getHCost();
    bool visited();
    bool isObstacle();
    int getRow();
    int getCol();

    void setGCost(int gcost);
    void setHCost(int hcost);
    void setVisited(bool status);
    void setObstacle(bool status);
    void setRow(int row);
    void setCol(int col);


    astarnode * getPredecessor();
    void setPredecessor(astarnode * predecessor);

};

#endif // _A_STAR_NODE_
