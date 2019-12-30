# astar
Visualization of the A* algorithm using SFML library

I chose to do this little side project during the 12/19 winter break. It gave me the chance to learn about SFML (Simple Fast Multimedia Library).

I was curious about A* for some time and decided to learn about it by first looking at Dijkstra's algorithm and then looking at examples of how A* performs its search.
I played around with different heuristics for A* (manhattan and euclidean) and using the manhattan distance I got the following results.

<p float="left" width="100%">
  <img src="https://github.com/anthoturc/astar/blob/master/beforePath.PNG" width="45%" />
  <img src="https://github.com/anthoturc/astar/blob/master/afterPath.PNG" width="45%" /> 
</p>

The source, desination and obstacles are chosen at run time. The green blocks represent the frontier of the searchm while the red nodes correspond to visited nodes.
The screen shots (above) allow for transitions along diagonals. Movement in only the cardinal directions is done by changing a macro.
