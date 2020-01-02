
#pragma once

#ifndef _VISUALIZATION_H_

#include <SFML/Graphics.hpp>

#include <vector>
#include <string>

#include "node.hpp"


/***********************************
*   settings for visualization
***********************************/

#define FAST_RENDER 0
#define SLEEP_TIME 2000000

#define W 1000
#define H 1000

#define BOX_W 25
#define BOX_H 25

#define BORDER_THICKNESS 1.5

#define BOX_SZ BOX_H

#define FRAME_RATE 60

#define RUN_KEY sf::Keyboard::R
#define RESET_KEY sf::Keyboard::E

/* costs for moving */
#define COST_MULTIPLIER 10

#define NSEW_BASE_COST 1
#define CARDINAL_COST (NSEW_BASE_COST * COST_MULTIPLIER)

#define DIAGONALS_ALLOWED 1 // determines valid directions
#define DIAGNOAL_BASE_COST 1.4 // this comes from sqrt((1)^2 + (1)^2)
#define DIAGNOAL_COST (int)(DIAGNOAL_BASE_COST * COST_MULTIPLIER)

class visualization
{
public:
    visualization(sf::RenderWindow& w);
    ~visualization();

    virtual void run() {}

protected:
    std::vector<std::vector<searchnode *>> grid_;
    sf::RenderWindow& w_;


    virtual void search() {}
    virtual void initGrid() {}
    virtual void resetGrid() {}
    void drawGrid();
    bool validNeighbor(int row, int col);

};

#endif // _VISUALIZATION_H_
