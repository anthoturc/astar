// TODO: make the colors of the nodes variables and create different colors with different meanings
// TODO: play around with hashing the mouse clicks... on mouse click change the color of the sprite

/*
 *  A* visualization
 *
 *  Author: antho
 */

#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <queue>

#include "astarnode.hpp"



/* window width and height */
#define W 900
#define H 900

/* specs for each box that will be drawn */
#define BOX_W 30
#define BOX_H 30

/* thickness of border between nodes */
#define BORDER_THICKNESS 1.5

#define BOX_SZ BOX_H

/* frame rate */
#define FRAME_RATE 60

/* key press macros */
#define RUN_ASTAR_KEY sf::Keyboard::A
#define RESET_GRID sf::Keyboard::R

/* node colors */
#define SRC_COLOR sf::Color(51, 51, 255)
#define DEST_COLOR sf::Color(255,51,51)
#define DEFAULT_COLOR sf::Color(255, 250, 250)
#define VISITED_COLOR sf::Color(255, 0, 0)
#define NEIGHBOR_FOUND_COLOR sf::Color(0, 255, 0)
#define PATH_COLOR SRC_COLOR

/* application name */
#define NAME "A* Visualization"

/* used for debugging */
#define DEBUG 0

/* costs for moving */
#define COST_MULTIPLIER 10

#define NSEW_BASE_COST 1
#define CARDINAL_COST (NSEW_BASE_COST * COST_MULTIPLIER)

#define DIAGONALS_ALLOWED 0 // determines valid directions
#define DIAGNOAL_BASE_COST 1.4 // this comes from sqrt((1)^2 + (1)^2)
#define DIAGNOAL_COST (int)(DIAGNOAL_BASE_COST * COST_MULTIPLIER)

/* grid to hold the sprites */
std::vector<std::vector<astarnode *>> grid;

astarnode * source = nullptr;
astarnode * dest = nullptr;

volatile bool dragging = false;
volatile bool runningAStar = false;

/* 8 directions of each nodes neighbors */
#if DIAGONALS_ALLOWED
std::vector<std::vector<int>> dirs {{-1, 0}, {-1, -1}, {-1, 1}, {0, 1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}};
#else
std::vector<std::vector<int>> dirs {{-1, 0}, {0, 1}, {0, -1}, {1, 0}};
#endif // DIAGONALS_ALLOWED



void drawGrid(sf::RenderWindow& w);

/**** A* algorithm ****/
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

bool
validNeighbor(int row, int col)
{
    if (row < 0 || row >= (H/BOX_W)) return false;
    if (col < 0 || col >= (W/BOX_W)) return false;
    return true;
}

void
runAStar(sf::RenderWindow& w)
{

    /* initialize the source and destination nodes */

    std::priority_queue<astarnode *, std::vector<astarnode *>, comp> open;
    std::vector<astarnode *> closed;
    std::vector<astarnode *> inopen;

    source->setGCost(0);
    source->setHCost(INT_MAX);

    dest->setGCost(INT_MAX);
    dest->setHCost(0);

    open.push(source);
    inopen.push_back(source);

    while (!open.empty()) { // note this will be infinite if the destination is unreachable

        astarnode * curr = open.top();
        open.pop();

        inopen.erase( // remove from open
            std::remove_if(
                inopen.begin(),
                inopen.end(),
                [&curr](astarnode * p) {
                    return p->getRow() == curr->getRow() && p->getCol() == curr->getCol();
                }
            )
        );

        if (curr != source)
            curr->r_->setFillColor(VISITED_COLOR);

        if (curr == dest) break;
#if DEBUG
        std::cout << "curr not dest" << std::endl;
#endif // DEBUG
        closed.push_back(curr);

        int row = curr->getRow();
        int col = curr->getCol();

        int currGCost = curr->getGCost();

        for (auto d : dirs) {
            int neighborRow = row + d[0];
            int neighborCol = col + d[1];

            if (validNeighbor(neighborRow, neighborCol)) {
                astarnode * neighbor = grid[neighborRow][neighborCol];

                if (neighbor->isObstacle()) continue;

                // calculate the g-cost for the neighboring node
                int gcost = CARDINAL_COST;

#if DIAGONALS_ALLOWED
                bool isDiagonal = (d[0] == 1 && d[1] == 1) || (d[0] == 1 && d[1] == -1)
                    || (d[0] == -1 && d[1] == -1) || (d[0] == -1 && d[1] == 1);

                if (isDiagonal) {
                    gcost = DIAGNOAL_COST;
                }
#endif // DIAGNOALS_ALLOWED


                gcost += currGCost;

                // calculate the h-cost for the neighboring nodes
                int x = neighborCol - col;
                int y = neighborRow - row;
                // use the euclidean distance as the h-cost
                int hcost = ((int)sqrt(pow((double)x, 2) + pow((double)y, 2)))*COST_MULTIPLIER;

                int fcost = hcost + gcost;

                auto itOpen = std::find(inopen.begin(), inopen.end(), neighbor);
                auto itClosed = std::find(closed.begin(), closed.end(), neighbor);


                if (itOpen != inopen.end() && gcost < neighbor->getGCost()) {

                    std::vector<astarnode *> tmp;

                    while (!open.empty()) {
                        if (open.top() != neighbor) {
                            tmp.push_back(open.top());

                        }
                        open.pop();
                    }

                    for (auto node : tmp) {
                        open.push(node);
                    }

                    inopen.erase( // remove from open
                        std::remove_if(
                            inopen.begin(),
                            inopen.end(),
                            [&curr](astarnode * p) {
                                return p->getRow() == curr->getRow() && p->getCol() == curr->getCol();
                            }
                        )
                    );

                    neighbor->r_->setFillColor(DEFAULT_COLOR);
                }

                if (itClosed != closed.end() && gcost < neighbor->getGCost()) {
                    closed.erase( // remove from open
                        std::remove_if(
                            closed.begin(),
                            closed.end(),
                            [&curr](astarnode * p) {
                                return p->getRow() == curr->getRow() && p->getCol() == curr->getCol();
                            }
                        )
                    );
                    neighbor->r_->setFillColor(DEFAULT_COLOR);
                }

                itOpen = std::find(inopen.begin(), inopen.end(), neighbor);
                itClosed = std::find(closed.begin(), closed.end(), neighbor);

                if (itOpen == inopen.end() && itClosed == closed.end())  {
                    // set costs
                    neighbor->setPredecessor(curr);
                    neighbor->setGCost(gcost);
                    neighbor->setHCost(hcost);

                    neighbor->r_->setFillColor(NEIGHBOR_FOUND_COLOR);

                    // add to open
                    open.push(neighbor);
                    inopen.push_back(neighbor);
                }
            }
        }
        w.clear();
        drawGrid(w);
        w.display();

    }

    w.clear();
    runningAStar = false;

    astarnode * tmp = dest;
    while (tmp) {
        tmp->r_->setFillColor(PATH_COLOR);
        tmp = tmp->getPredecessor();
    }
    drawGrid(w);
    w.display();
}

/**** grid initialization ****/
void
initGrid()
{
    int numRows = H/BOX_H;
    int numCols = W/BOX_W;

    grid.resize(numRows, std::vector<astarnode *>(numCols));

    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            int xpos = row * BOX_SZ;
            int ypos = col * BOX_SZ;
            if (DEBUG)
                std::cout << xpos << ", " << ypos << std::endl;

            sf::RectangleShape * r = new sf::RectangleShape(sf::Vector2f(BOX_W, BOX_H));

            r->setOutlineColor(sf::Color::Black);
            r->setOutlineThickness(BORDER_THICKNESS);
            r->setFillColor(DEFAULT_COLOR);

            r->setPosition((float)xpos, (float)ypos);
            grid[row][col] = new astarnode(r);

            grid[row][col]->setRow(row);
            grid[row][col]->setCol(col);
        }
    }
}
/***********/

/**** grid reset ****/
void
resetGrid()
{
    if (DEBUG)
        std::cout << "going to reset" << std::endl;

    source = dest = nullptr;
    dragging = false;
    runningAStar = false;

    for (auto row : grid) {
        for (auto node : row) {
            node->setGCost(INT_MAX);
            node->setHCost(INT_MAX);
            node->setObstacle(false);
            node->setPredecessor(nullptr);
            node->r_->setFillColor(DEFAULT_COLOR);
        }
    }
}
/************/

/**** grid modifications ****/
astarnode *
setColor(int row, int col, const sf::Color& c)
{
    auto node = grid[row][col];
    node->r_->setFillColor(c);
    return node;
}

astarnode *
setNodeColor(int xpos, int ypos, const sf::Color& c)
{
    int row = xpos/BOX_SZ;
    int col = ypos/BOX_SZ;

    return setColor(row, col, c);
}

void
setObstacle(sf::RenderWindow& w, sf::Event& e)
{
    int xpos = e.mouseMove.x;
    int ypos = e.mouseMove.y;

    if (xpos >= W || xpos < 0) return;
    if (ypos >= H || ypos < 0) return;

    if (DEBUG)
        std::cout << xpos << ", " << ypos << std::endl;

    astarnode * obstacle = setNodeColor(xpos, ypos, sf::Color::Black);
    obstacle->setObstacle(true);
}
/***********/

/**** left/right button clicks ****/
void
handleMouseDownEvent(sf::RenderWindow& w, sf::Event& e)
{
    int xpos = e.mouseButton.x;
    int ypos = e.mouseButton.y;

    if (xpos < 0 || xpos >= W) return;
    if (ypos < 0 || ypos >= H) return;

    if (!dragging) {
        // TODO: check if dragging happens when expected
        if (!source && e.mouseButton.button == sf::Mouse::Left) { // source node set
            source = setNodeColor(xpos, ypos, SRC_COLOR);
        } else if (!dest && e.mouseButton.button == sf::Mouse::Right) { // destination node set
            dest = setNodeColor(xpos, ypos, DEST_COLOR);
        }
    }
}
/***********/

/**** key presses ****/
void
handleKeyPress(sf::Event& e)
{
    if (e.key.code == RUN_ASTAR_KEY) {
        if (source && dest) {
            runningAStar = true;
        }
    } else if (e.key.code == RESET_GRID) {
        resetGrid();
    }
#if DEBUG
    std::cout << e.key.code << std::endl;
#endif // DEBUG
}

/**** window events ****/
void
handleWindowEvents(sf::RenderWindow& w)
{
    sf::Event e;
    while (w.pollEvent(e)) {

        switch (e.type) {
            case sf::Event::Closed:
                w.close();
                break;
            case sf::Event::MouseButtonPressed:
                if (runningAStar) break;
                if (!source || !dest)
                    handleMouseDownEvent(w, e);
                else
                    dragging = true;
                break;
            case sf::Event::MouseMoved:
                if (runningAStar) break;
                if (dragging && source && dest) // source and dest must be set before obstacles
                    setObstacle(w, e);
                break;
            case sf::Event::MouseButtonReleased:
                if (runningAStar) break;
                dragging = false;
                if (DEBUG)
                    std::cout << "mouse btn let go" << std::endl;
                break;
            case sf::Event::KeyPressed:
                handleKeyPress(e);
                break;
            default: // TODO: add other events here
                break;
        }
    }
}
/***********/

/**** drawing grid to the screen ****/
void
drawGrid(sf::RenderWindow& window)
{
    for (auto v : grid) {
        for (auto node : v) {
            window.draw(*(node->r_));
        }
    }
}
/***********/

int
main()
{
    sf::RenderWindow window(sf::VideoMode(W, H), NAME);
    window.setFramerateLimit(FRAME_RATE);

    initGrid();

    while (window.isOpen()) {

        handleWindowEvents(window);

        window.clear();
        drawGrid(window);
        if (!runningAStar)
            window.display();
        else
            runAStar(window);
    }

    return 0;
}
