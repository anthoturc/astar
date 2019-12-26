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

#include "astarnode.hpp"

/* window width and height */
#define W 600
#define H 600

/* specs for each box that will be drawn */
#define BOX_W 30
#define BOX_H 30

#define BOX_SZ BOX_H

/* frame rate */
#define FRAME_RATE 60

#define NODE_TEXTURE "box.png"

/* application name */
#define NAME "A* Visualization"

/* used for debugging */
#define DEBUG 0

/* grid to hold the sprites */
std::vector<std::vector<astarnode *>> grid;

void
initGrid()
{
    int numRows = H/BOX_H;
    int numCols = W/BOX_W;

    grid.resize(numRows, std::vector<astarnode *>(numCols));

    sf::Texture t;
    if (!t.loadFromFile(NODE_TEXTURE)) {
        std::cout << "main.cpp @ 42: Unable to load " << NODE_TEXTURE << std::endl;
        exit(1);
    }
    t.setSmooth(true);
    t.setRepeated(false);

    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            // assigning position this way allows for hashing
            // of mouse clicks later
            int xpos = row * BOX_SZ;
            int ypos = col * BOX_SZ;
            if (DEBUG)
                std::cout << xpos << ", " << ypos << std::endl;

            sf::RectangleShape * r = new sf::RectangleShape(sf::Vector2f(BOX_W, BOX_H));

            r->setOutlineColor(sf::Color::Black);
            r->setOutlineThickness(1);
            r->setFillColor(sf::Color(255, 128, 0));

            r->setPosition((float)xpos, (float)ypos);
            grid[row][col] = new astarnode(r);
        }
    }
}

void
handleWindowEvents(sf::RenderWindow& w)
{
    sf::Event e;
    while (w.pollEvent(e)) {
        if (e.type == sf::Event::Closed) {
            w.close();
        }
    }
}

void
drawGrid(sf::RenderWindow& window)
{
    for (auto v : grid) {
        for (auto node : v) {
            window.draw(*(node->r_));
        }
    }
}

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
        window.display();
    }

    return 0;
}
