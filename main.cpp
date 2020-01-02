/*
 *  A* visualization
 *
 *  Author: antho
 */

#include "astarvisualization.hpp"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <queue>
#include <unistd.h>

int
main()
{
    sf::RenderWindow w(sf::VideoMode(W, H), "A* Visualization");
    astarvisualization viz(w);
    viz.run();
    return 0;
}
