#ifndef _ASTAR_HG_
#define _ASTAR_HG_

#include "cNode.h"
#include <vector>

class AStar
{
public:
	AStar();
	bool LoadMap(std::vector<int>, int row, int col);
	bool DoAStar();
	bool DoAStar8Way();

	cNode *startNode;
	cNode *endNode;
	std::vector<cNode*> shortestPath;
	std::vector<std::pair<float, cNode>> distanceList;
	bool doIter;
private:
	bool isValid(int row, int col);
	bool isUnblocked(int row, int col);
	bool isDestination(int row, int col, cNode dest);
	bool calculateH(int row, int col, cNode dest);
	std::vector<cNode*> tracePath(cNode dest);


	int **Map;
	int Row;
	int Col;
	bool destFound;
	

	std::vector<cNode*> mapInfo;
};

#endif // !_ASTAR_HG_
