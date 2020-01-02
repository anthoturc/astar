#include "astarvisualization.hpp"
#include <queue>
#include <unistd.h>
#include <vector>
#include <algorithm>

astarvisualization::astarvisualization(sf::RenderWindow& w)
    : visualization(w), source_(nullptr), dest_(nullptr), runningAStar_(false), dragging_(false) {}

astarvisualization::~astarvisualization() {}

int
astarvisualization::manhattan_heuristic(astarnode* n1, astarnode* n2)
{
    return COST_MULTIPLIER*(
        std::abs(n1->getCol() - n2->getCol())
    +   std::abs(n1->getRow() - n2->getRow())
    );
}

void
astarvisualization::search()
{
    /* 8 directions of each nodes neighbors */
#if DIAGONALS_ALLOWED
    std::vector<std::vector<int>> dirs {{-1, 0}, {-1, -1}, {-1, 1}, {0, 1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}};
#else
    std::vector<std::vector<int>> dirs {{-1, 0}, {0, 1}, {0, -1}, {1, 0}};
#endif // DIAGONALS_ALLOWED
    std::priority_queue<astarnode *, std::vector<astarnode *>, comp> open;
    std::vector<astarnode *> inopen;

    source_->setGCost(0);
    source_->setHCost(INT_MAX);

    dest_->setGCost(INT_MAX);
    dest_->setHCost(0);

    open.push(source_);
    inopen.push_back(source_);

    while (!open.empty()) { // note this will be infinite if the destination is unreachable

        astarnode * curr = open.top();
        open.pop();

        if (curr != source_ && curr != dest_)
            curr->r_->setFillColor(VISITED_COLOR);
#if FAST_RENDER

#else
        w_.clear();
        drawGrid();
        w_.display();
#endif // FAST_RENDER

        if (curr == dest_) break;

        int row = curr->getRow();
        int col = curr->getCol();

        int currGCost = curr->getGCost();

        for (auto d : dirs) {

            int neighborRow = row + d[0];
            int neighborCol = col + d[1];

            if (validNeighbor(neighborRow, neighborCol)) {
                astarnode * neighbor = (astarnode *)grid_[neighborRow][neighborCol];

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

                auto itOpen = std::find(inopen.begin(), inopen.end(), neighbor);

                if (itOpen == inopen.end() || gcost < neighbor->getGCost()) {


                    int hcost = manhattan_heuristic(neighbor, dest_);

                    // set costs
                    neighbor->setPredecessor(curr);
                    neighbor->setGCost(gcost);
                    neighbor->setHCost(hcost);
                    if (neighbor != dest_)
                        neighbor->r_->setFillColor(NEIGHBOR_FOUND_COLOR);
                    // add to open
                    open.push(neighbor);
                    inopen.push_back(neighbor);
                }
            }
        }
#if FAST_RENDER
#else
        w_.clear();
        drawGrid();
        w_.display();
        usleep(SLEEP_TIME/2);
#endif // FAST_RENDER

    }

    w_.clear();
    runningAStar_ = false;
    if (!dest_->getPredecessor()) {
        dest_->r_->setFillColor(PATH_NOT_FOUND_COLOR);
        drawGrid();
        w_.display();
        return;
    }

    astarnode * tmp = dest_;
    while (tmp) {
        if (tmp != dest_ && tmp != source_)
            tmp->r_->setFillColor(PATH_COLOR);
        tmp = tmp->getPredecessor();
        drawGrid();
        usleep(SLEEP_TIME); // sleep for 1 sec
        w_.display();
    }
}

void
astarvisualization::initGrid()
{
    int numRows = H/BOX_H;
    int numCols = W/BOX_W;

    grid_.resize(numRows, std::vector<searchnode *>(numCols));

    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            int xpos = row * BOX_SZ;
            int ypos = col * BOX_SZ;

            sf::RectangleShape * r = new sf::RectangleShape(sf::Vector2f(BOX_W, BOX_H));

            r->setOutlineColor(sf::Color::Black);
            r->setOutlineThickness(BORDER_THICKNESS);
            r->setFillColor(DEFAULT_COLOR);
            r->setPosition((float)xpos, (float)ypos);

            grid_[row][col] = new astarnode();
            grid_[row][col]->r_ = r;

            grid_[row][col]->setRow(row);
            grid_[row][col]->setCol(col);
        }
    }
}

void
astarvisualization::resetGrid()
{
    source_ = dest_ = nullptr;
    dragging_ = runningAStar_ = false;

    for (auto row : grid_) {
        for (auto node : row) {
            astarnode * tmp = (astarnode *)node;
            tmp->setGCost(INT_MAX);
            tmp->setHCost(INT_MAX);
            tmp->setObstacle(false);
            tmp->setPredecessor(nullptr);
            tmp->r_->setFillColor(DEFAULT_COLOR);
        }
    }
}

void
astarvisualization::run()
{
    initGrid();

    while (w_.isOpen()) {
        handleWindowEvents();
        w_.clear();
        drawGrid();
        if (!runningAStar_) {
            w_.display();
        } else {
            search();
        }
    }
}

void
astarvisualization::handleWindowEvents()
{
    sf::Event e;
    while (w_.pollEvent(e)) {
        switch (e.type) {
            case sf::Event::Closed:
                w_.close();
                break;
            case sf::Event::MouseButtonPressed:
                if (runningAStar_) break;
                if (!source_ || !dest_)
                    handleMouseDownEvent(e);
                else
                    dragging_ = true;
                break;
            case sf::Event::MouseMoved:
                if (runningAStar_) break;
                if (dragging_ && source_ && dest_) // source and dest must be set before obstacles
                    setObstacle(e);
                break;
            case sf::Event::MouseButtonReleased:
                if (runningAStar_) break;
                dragging_ = false;
                break;
            case sf::Event::KeyPressed:
                handleKeyPress(e);
                break;
            default: // TODO: add other events here
                break;
        }
    }
}

void
astarvisualization::handleKeyPress(sf::Event& e)
{
    if (e.key.code == RUN_KEY) {
        if (source_ && dest_) {
            runningAStar_ = true;
        }
    } else if (e.key.code == RESET_KEY) {
        resetGrid();
    }
}

void
astarvisualization::handleMouseDownEvent(sf::Event& e)
{
    int xpos = e.mouseButton.x;
    int ypos = e.mouseButton.y;

    if (xpos < 0 || xpos >= W) return;
    if (ypos < 0 || ypos >= H) return;

    if (!dragging_) {
        if (!source_ && e.mouseButton.button == sf::Mouse::Left) { // source node set
            source_ = setNodeColor(xpos, ypos, SRC_COLOR);
        } else if (!dest_ && e.mouseButton.button == sf::Mouse::Right) { // destination node set
            dest_ = setNodeColor(xpos, ypos, DEST_COLOR);
        }
    }
}

astarnode *
astarvisualization::setNodeColor(int xpos, int ypos, const sf::Color& c)
{
    int row = xpos/BOX_SZ;
    int col = ypos/BOX_SZ;

    grid_[row][col]->r_->setFillColor(c);
    return (astarnode *)grid_[row][col];
}

void
astarvisualization::setObstacle(sf::Event& e)
{
    int xpos = e.mouseMove.x;
    int ypos = e.mouseMove.y;

    if (xpos >= W || xpos < 0) return;
    if (ypos >= H || ypos < 0) return;

    astarnode * obstacle = setNodeColor(xpos, ypos, sf::Color::Black);
    if (obstacle == source_) {
        setNodeColor(xpos, ypos, SRC_COLOR);
        return;
    } else if (obstacle == dest_) {
        setNodeColor(xpos, ypos, DEST_COLOR);
    }
    obstacle->setObstacle(true);
}
