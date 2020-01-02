
#pragma once

#ifndef _A_STAR_NODE_
#define _A_STAR_NODE_

#include <SFML/Graphics.hpp>
#include "node.hpp"

class astarnode : public searchnode
{
private:
    int gcost_;
    int hcost_;
    bool isobstacle_;
    astarnode * predecessor_;

public:

    astarnode();
    ~astarnode();

    int getFCost();

    int getGCost();
    int getHCost();
    bool isObstacle();

    void setGCost(int gcost);
    void setHCost(int hcost);
    void setObstacle(bool status);

    astarnode * getPredecessor();
    void setPredecessor(astarnode * predecessor);

};

#endif // _A_STAR_NODE_
