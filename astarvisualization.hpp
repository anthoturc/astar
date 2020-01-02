
#pragma once

#ifndef _ASTAR_VISUALIZATION_H_

#include <string>
#include <queue>
#include <vector>

#include "visualization.hpp"
#include "astarnode.hpp"

#define SRC_COLOR sf::Color(51, 51, 255)
#define DEST_COLOR SRC_COLOR
#define DEFAULT_COLOR sf::Color(255, 250, 250)
#define VISITED_COLOR sf::Color(255, 0, 0)
#define NEIGHBOR_FOUND_COLOR sf::Color(0, 255, 0)
#define PATH_COLOR SRC_COLOR
#define PATH_NOT_FOUND_COLOR sf::Color::Magenta

struct comp
{
    inline bool operator()(astarnode * lhs, astarnode * rhs) {
        int lFCost = lhs->getFCost();
        int rFCost = rhs->getFCost();

        if (lFCost == rFCost) {
            return lhs->getGCost() > rhs->getGCost();
        }

        return lFCost > rFCost;
    }
};

class astarvisualization : public visualization
{
public:
    astarvisualization(sf::RenderWindow& w);
    ~astarvisualization();

    void run();

private:
    astarnode * source_;
    astarnode * dest_;
    bool runningAStar_;
    bool dragging_;

    /* event handlers */
    void handleWindowEvents();
    void handleKeyPress(sf::Event& e);
    void handleMouseDownEvent(sf::Event& e);

    /* search methods */
    void search();
    void initGrid();
    void resetGrid();
    int manhattan_heuristic(astarnode* n1, astarnode* n2);

    /* node modification */
    astarnode * setNodeColor(int xpos, int ypos, const sf::Color& c);
    void setObstacle(sf::Event& e);
};


#endif // _ASTAR_VISUALIZATION_H_

