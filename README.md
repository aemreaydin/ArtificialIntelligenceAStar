## Artificial Intelligence

#### PROJECT #1 - A* Algorithm

The project uses A* Algorithm to find the shortest path in a maze.

Map file is inside the assets folder. Can be changed to any complexity.

Brown blocks are passable terrain. Yellow blocks are obstacles. Green blocks show the shortest path.

The models are created using Blender with textures and imported using Assimp. There were different textures for each box. However, there is a problem with importing textures with blender as it only outputs the color. (Needs fixing.)

x64 - Release/Debug -> Working properly.

! Moves between points, however, rotation is not working properly. Rotates only once and it might or might not be a correct rotation :).

! Rotation removed.



##### Controls

* SPACE ---- Run A* algorithm. It will output the shortest path in the maze.
* ENTER ---- Run A* algorithm with 8 directions(N-S-E-W-NE-NW-SE-SW). It will output the shortest path in the maze with diagonal movement. Movement is somewhat iffy with this one.
* M ---- Moves the AI character to the next point in the maze.(Move AI after finding the shortest path.)
* C ---- Changes the camera settings between 3rd person and free view.
* WASDEQ ---- Controls the camera movements.
* Mouse ---- Can be used with the free view camera.

! Space or Enter can be used once per program. Meaning, you have to restart the program if you want to use AStar8Way after using AStar or vice versa.