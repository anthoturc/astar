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

#define SRC_COLOR sf::Color(51, 51, 255)
#define DEST_COLOR sf::Color(255, 51, 51)

/* application name */
#define NAME "A* Visualization"

/* used for debugging */
#define DEBUG 0

/* grid to hold the sprites */
std::vector<std::vector<astarnode *>> grid;

astarnode * source = nullptr;
astarnode * dest = nullptr;

volatile bool dragging = false;

void
initGrid()
{
    int numRows = H/BOX_H;
    int numCols = W/BOX_W;

    grid.resize(numRows, std::vector<astarnode *>(numCols));

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

astarnode *
setNodeColor(int xpos, int ypos, const sf::Color& c)
{
    int row = xpos/BOX_SZ;
    int col = ypos/BOX_SZ;

    auto node = grid[row][col];
    node->r_->setFillColor(c);
    return node;
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
    obstacle->setToObstacle();
}

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
                if (!source || !dest)
                    handleMouseDownEvent(w, e);
                else
                    dragging = true;
                break;
            case sf::Event::MouseMoved:
                if (dragging && source && dest) // source and dest must be set before obstacles
                    setObstacle(w, e);
                break;
            case sf::Event::MouseButtonReleased:
                dragging = false;
                if (DEBUG)
                    std::cout << "mouse btn let go" << std::endl;
                break;
            default: // TODO: add other events here
                break;
        }
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
