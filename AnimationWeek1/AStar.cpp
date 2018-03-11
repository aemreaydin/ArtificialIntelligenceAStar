#include "AStar.h"

AStar::AStar()
{
	destFound = false;
	startNode = new cNode;
	endNode = new cNode;
	doIter = false;
}

bool AStar::LoadMap(std::vector<int> map, const int row, const int col)
{
	this->Row = row;
	this->Col = col;
	int** newMap = new int*[row];
	for (int i = 0; i != row; ++i)
		newMap[i] = new int[col];

	this->Map = newMap;
	for (int i = 0; i != this->Row; i++)
	{
		for (int j = 0; j != this->Col; j++)
		{
			this->Map[i][j] = map[i * col + j];
		}
	}

	return true;
}

bool AStar::DoAStar()
{
	this->shortestPath.clear();
	if (this->isValid(startNode->i, startNode->j) == false)
	{
		printf("Source is invalid\n");
		return false;
	}
	if (this->isValid(endNode->i, endNode->j) == false)
	{
		printf("Destination is invalid\n");
		return false;
	}
	if (this->isUnblocked(startNode->i, startNode->j) == false
		|| this->isUnblocked(endNode->i, endNode->j) == false)
	{
		printf("Source or destination is blocked\n");
		return false;
	}
	if (isDestination(startNode->i, startNode->j, *endNode) == true)
	{
		printf("We are already at the destination\n");
		return false;
	}

	// Create and initialize closed list
	bool** closedList = new bool*[this->Row];
	for (int i = 0; i != this->Row; ++i)
		closedList[i] = new bool[this->Col];
	for (int i = 0; i != this->Row; i++)
	{
		for (int j = 0; j != this->Col; j++)
			closedList[i][j] = false;
	}

	for (int i = 0; i != this->Row; i++)
	{
		for (int j = 0; j != this->Col; j++)
		{
			cNode* newNode = new cNode;
			newNode->f = FLT_MAX;
			newNode->g = FLT_MAX;
			newNode->h = FLT_MAX;
			newNode->i = -1;
			newNode->j = -1;
			this->mapInfo.push_back(newNode);
		}
	}
	mapInfo[startNode->i * this->Col + startNode->j]->f = 0.0f;
	mapInfo[startNode->i * this->Col + startNode->j]->g = 0.0f;
	mapInfo[startNode->i * this->Col + startNode->j]->h = 0.0f;
	mapInfo[startNode->i * this->Col + startNode->j]->i = startNode->i;
	mapInfo[startNode->i * this->Col + startNode->j]->j = startNode->j;

	distanceList.push_back(std::make_pair(0.0f, *startNode));

	int i, j;
	while (!distanceList.empty() && doIter)
	{
		std::pair<float, cNode> curPair = *distanceList.begin();
		distanceList.erase(distanceList.begin());


		i = curPair.second.i;
		j = curPair.second.j;
		closedList[curPair.second.i][curPair.second.j] = true;

		float newG, newH, newF;
		// North
		if (this->isValid(i - 1, j))
		{
			if (isDestination(i - 1, j, *endNode))
			{
				mapInfo[(i - 1) * this->Col + j]->i = i;
				mapInfo[(i - 1) * this->Col + j]->j = j;
				printf("The destination cell is found\n");
				this->shortestPath = tracePath(*endNode);
				this->destFound = true;
				return true;
			}
			else if (closedList[i - 1][j] == false && isUnblocked(i - 1, j) == true)
			{
				newG = mapInfo[i * this->Col + j]->g + 1.0f;
				newH = calculateH(i - 1, j, *endNode);
				newF = newG + newH;

				if (mapInfo[(i - 1) * this->Col + j]->f == FLT_MAX || mapInfo[(i - 1) * this->Col + j]->f > newF)
				{
					cNode inNode;
					inNode.i = i - 1;
					inNode.j = j;
					distanceList.push_back(std::make_pair(newF, inNode));
					mapInfo[(i - 1) * this->Col + j]->f = newF;
					mapInfo[(i - 1) * this->Col + j]->g = newG;
					mapInfo[(i - 1) * this->Col + j]->h = newH;
					mapInfo[(i - 1) * this->Col + j]->i = i;
					mapInfo[(i - 1) * this->Col + j]->j = j;
				}
			}
		}
		// South
		if (this->isValid(i + 1, j))
		{
			if (isDestination(i + 1, j, *endNode))
			{
				mapInfo[(i + 1) * this->Col + j]->i = i;
				mapInfo[(i + 1) * this->Col + j]->j = j;
				printf("The destination cell is found\n");
				this->shortestPath = tracePath(*endNode);
				this->destFound = true;
				return true;
			}
			else if (closedList[i + 1][j] == false && isUnblocked(i + 1, j) == true)
			{
				newG = mapInfo[i * this->Col + j]->g + 1.0f;
				newH = calculateH(i + 1, j, *endNode);
				newF = newG + newH;

				if (mapInfo[(i + 1) * this->Col + j]->f == FLT_MAX || mapInfo[(i + 1) * this->Col + j]->f > newF)
				{
					cNode inNode;
					inNode.i = i + 1;
					inNode.j = j;
					distanceList.push_back(std::make_pair(newF, inNode));
					mapInfo[(i + 1) * this->Col + j]->f = newF;
					mapInfo[(i + 1) * this->Col + j]->g = newG;
					mapInfo[(i + 1) * this->Col + j]->h = newH;
					mapInfo[(i + 1) * this->Col + j]->i = i;
					mapInfo[(i + 1) * this->Col + j]->j = j;
				}
			}
		}
		// East
		if (this->isValid(i, j + 1))
		{
			if (isDestination(i, j + 1, *endNode))
			{
				mapInfo[(i)* this->Col + j + 1]->i = i;
				mapInfo[(i)* this->Col + j + 1]->j = j;
				printf("The destination cell is found\n");
				this->shortestPath = tracePath(*endNode);
				this->destFound = true;
				return true;
			}
			else if (closedList[i][j + 1] == false && isUnblocked(i, j + 1) == true)
			{
				newG = mapInfo[i * this->Col + j]->g + 1.0f;
				newH = calculateH(i, j + 1, *endNode);
				newF = newG + newH;

				if (mapInfo[(i)* this->Col + j + 1]->f == FLT_MAX || mapInfo[(i)* this->Col + j + 1]->f > newF)
				{
					cNode inNode;
					inNode.i = i;
					inNode.j = j + 1;
					distanceList.push_back(std::make_pair(newF, inNode));
					mapInfo[(i)* this->Col + (j + 1)]->f = newF;
					mapInfo[(i)* this->Col + (j + 1)]->g = newG;
					mapInfo[(i)* this->Col + (j + 1)]->h = newH;
					mapInfo[(i)* this->Col + (j + 1)]->i = i;
					mapInfo[(i)* this->Col + (j + 1)]->j = j;
				}
			}
		}
		// West
		if (this->isValid(i, j - 1))
		{
			if (isDestination(i, j - 1, *endNode))
			{
				mapInfo[(i)* this->Col + j - 1]->i = i;
				mapInfo[(i)* this->Col + j - 1]->j = j;
				printf("The destination cell is found\n");
				this->shortestPath = tracePath(*endNode);
				this->destFound = true;
				return true;
			}
			else if (closedList[i][j - 1] == false && isUnblocked(i, j - 1) == true)
			{
				newG = mapInfo[i * this->Col + j]->g + 1.0f;
				newH = calculateH(i, j - 1, *endNode);
				newF = newG + newH;

				if (mapInfo[(i)* this->Col + j - 1]->f == FLT_MAX || mapInfo[(i)* this->Col + j - 1]->f > newF)
				{
					cNode inNode;
					inNode.i = i;
					inNode.j = j - 1;
					distanceList.push_back(std::make_pair(newF, inNode));
					mapInfo[(i)* this->Col + (j - 1)]->f = newF;
					mapInfo[(i)* this->Col + (j - 1)]->g = newG;
					mapInfo[(i)* this->Col + (j - 1)]->h = newH;
					mapInfo[(i)* this->Col + (j - 1)]->i = i;
					mapInfo[(i)* this->Col + (j - 1)]->j = j;
				}
			}
		}
	}
	if (this->destFound == false && doIter == true)
		printf("Failed to find the destination.\n");
	return false;
}

bool AStar::DoAStar8Way()
{
	this->shortestPath.clear();
	if (this->isValid(startNode->i, startNode->j) == false)
	{
		printf("Source is invalid\n");
		return false;
	}
	if (this->isValid(endNode->i, endNode->j) == false)
	{
		printf("Destination is invalid\n");
		return false;
	}
	if (this->isUnblocked(startNode->i, startNode->j) == false
		|| this->isUnblocked(endNode->i, endNode->j) == false)
	{
		printf("Source or destination is blocked\n");
		return false;
	}
	if (isDestination(startNode->i, startNode->j, *endNode) == true)
	{
		printf("We are already at the destination\n");
		return false;
	}

	// Create and initialize closed list
	bool** closedList = new bool*[this->Row];
	for (int i = 0; i != this->Row; ++i)
		closedList[i] = new bool[this->Col];
	for (int i = 0; i != this->Row; i++)
	{
		for (int j = 0; j != this->Col; j++)
			closedList[i][j] = false;
	}

	for (int i = 0; i != this->Row; i++)
	{
		for (int j = 0; j != this->Col; j++)
		{
			cNode* newNode = new cNode;
			newNode->f = FLT_MAX;
			newNode->g = FLT_MAX;
			newNode->h = FLT_MAX;
			newNode->i = -1;
			newNode->j = -1;
			this->mapInfo.push_back(newNode);
		}
	}
	mapInfo[startNode->i * this->Col + startNode->j]->f = 0.0f;
	mapInfo[startNode->i * this->Col + startNode->j]->g = 0.0f;
	mapInfo[startNode->i * this->Col + startNode->j]->h = 0.0f;
	mapInfo[startNode->i * this->Col + startNode->j]->i = startNode->i;
	mapInfo[startNode->i * this->Col + startNode->j]->j = startNode->j;

	distanceList.push_back(std::make_pair(0.0f, *startNode));

	int i, j;
	while (!distanceList.empty() && doIter)
	{
		std::pair<float, cNode> curPair = *distanceList.begin();
		distanceList.erase(distanceList.begin());


		i = curPair.second.i;
		j = curPair.second.j;
		closedList[curPair.second.i][curPair.second.j] = true;

		float newG, newH, newF;
		// North
		if (this->isValid(i - 1, j))
		{
			if (isDestination(i - 1, j, *endNode))
			{
				mapInfo[(i - 1) * this->Col + j]->i = i;
				mapInfo[(i - 1) * this->Col + j]->j = j;
				printf("The destination cell is found\n");
				this->shortestPath = tracePath(*endNode);
				this->destFound = true;
				return true;
			}
			else if (closedList[i - 1][j] == false && isUnblocked(i - 1, j) == true)
			{
				newG = mapInfo[i * this->Col + j]->g + 1.0f;
				newH = calculateH(i - 1, j, *endNode);
				newF = newG + newH;

				if (mapInfo[(i - 1) * this->Col + j]->f == FLT_MAX || mapInfo[(i - 1) * this->Col + j]->f > newF)
				{
					cNode inNode;
					inNode.i = i - 1;
					inNode.j = j;
					distanceList.push_back(std::make_pair(newF, inNode));
					mapInfo[(i - 1) * this->Col + j]->f = newF;
					mapInfo[(i - 1) * this->Col + j]->g = newG;
					mapInfo[(i - 1) * this->Col + j]->h = newH;
					mapInfo[(i - 1) * this->Col + j]->i = i;
					mapInfo[(i - 1) * this->Col + j]->j = j;
				}
			}
		}
		// South
		if (this->isValid(i + 1, j))
		{
			if (isDestination(i + 1, j, *endNode))
			{
				mapInfo[(i + 1) * this->Col + j]->i = i;
				mapInfo[(i + 1) * this->Col + j]->j = j;
				printf("The destination cell is found\n");
				this->shortestPath = tracePath(*endNode);
				this->destFound = true;
				return true;
			}
			else if (closedList[i + 1][j] == false && isUnblocked(i + 1, j) == true)
			{
				newG = mapInfo[i * this->Col + j]->g + 1.0f;
				newH = calculateH(i + 1, j, *endNode);
				newF = newG + newH;

				if (mapInfo[(i + 1) * this->Col + j]->f == FLT_MAX || mapInfo[(i + 1) * this->Col + j]->f > newF)
				{
					cNode inNode;
					inNode.i = i + 1;
					inNode.j = j;
					distanceList.push_back(std::make_pair(newF, inNode));
					mapInfo[(i + 1) * this->Col + j]->f = newF;
					mapInfo[(i + 1) * this->Col + j]->g = newG;
					mapInfo[(i + 1) * this->Col + j]->h = newH;
					mapInfo[(i + 1) * this->Col + j]->i = i;
					mapInfo[(i + 1) * this->Col + j]->j = j;
				}
			}
		}
		// East
		if (this->isValid(i, j + 1))
		{
			if (isDestination(i, j + 1, *endNode))
			{
				mapInfo[(i)* this->Col + j + 1]->i = i;
				mapInfo[(i)* this->Col + j + 1]->j = j;
				printf("The destination cell is found\n");
				this->shortestPath = tracePath(*endNode);
				this->destFound = true;
				return true;
			}
			else if (closedList[i][j + 1] == false && isUnblocked(i, j + 1) == true)
			{
				newG = mapInfo[i * this->Col + j]->g + 1.0f;
				newH = calculateH(i, j + 1, *endNode);
				newF = newG + newH;

				if (mapInfo[(i)* this->Col + j + 1]->f == FLT_MAX || mapInfo[(i)* this->Col + j + 1]->f > newF)
				{
					cNode inNode;
					inNode.i = i;
					inNode.j = j + 1;
					distanceList.push_back(std::make_pair(newF, inNode));
					mapInfo[(i)* this->Col + (j + 1)]->f = newF;
					mapInfo[(i)* this->Col + (j + 1)]->g = newG;
					mapInfo[(i)* this->Col + (j + 1)]->h = newH;
					mapInfo[(i)* this->Col + (j + 1)]->i = i;
					mapInfo[(i)* this->Col + (j + 1)]->j = j;
				}
			}
		}
		// West
		if (this->isValid(i, j - 1))
		{
			if (isDestination(i, j - 1, *endNode))
			{
				mapInfo[(i)* this->Col + j - 1]->i = i;
				mapInfo[(i)* this->Col + j - 1]->j = j;
				printf("The destination cell is found\n");
				this->shortestPath = tracePath(*endNode);
				this->destFound = true;
				return true;
			}
			else if (closedList[i][j - 1] == false && isUnblocked(i, j - 1) == true)
			{
				newG = mapInfo[i * this->Col + j]->g + 1.0f;
				newH = calculateH(i, j - 1, *endNode);
				newF = newG + newH;

				if (mapInfo[(i)* this->Col + j - 1]->f == FLT_MAX || mapInfo[(i)* this->Col + j - 1]->f > newF)
				{
					cNode inNode;
					inNode.i = i;
					inNode.j = j - 1;
					distanceList.push_back(std::make_pair(newF, inNode));
					mapInfo[(i)* this->Col + (j - 1)]->f = newF;
					mapInfo[(i)* this->Col + (j - 1)]->g = newG;
					mapInfo[(i)* this->Col + (j - 1)]->h = newH;
					mapInfo[(i)* this->Col + (j - 1)]->i = i;
					mapInfo[(i)* this->Col + (j - 1)]->j = j;
				}
			}
		}
		// NorthEast
		if (this->isValid(i - 1, j + 1))
		{
			if (isDestination(i - 1, j + 1, *endNode))
			{
				mapInfo[(i - 1) * this->Col + j + 1]->i = i;
				mapInfo[(i - 1) * this->Col + j + 1]->j = j;
				printf("The destination cell is found\n");
				this->shortestPath = tracePath(*endNode);
				this->destFound = true;
				return true;
			}
			else if (closedList[i - 1][j + 1] == false && isUnblocked(i - 1, j + 1) == true)
			{
				newG = mapInfo[i * this->Col + j]->g + 1.0f;
				newH = calculateH(i - 1, j + 1, *endNode);
				newF = newG + newH;

				if (mapInfo[(i - 1) * this->Col + j + 1]->f == FLT_MAX || mapInfo[(i - 1) * this->Col + j + 1]->f > newF)
				{
					cNode inNode;
					inNode.i = i - 1;
					inNode.j = j + 1;
					distanceList.push_back(std::make_pair(newF, inNode));
					mapInfo[(i - 1) * this->Col + j + 1]->f = newF;
					mapInfo[(i - 1) * this->Col + j + 1]->g = newG;
					mapInfo[(i - 1) * this->Col + j + 1]->h = newH;
					mapInfo[(i - 1) * this->Col + j + 1]->i = i;
					mapInfo[(i - 1) * this->Col + j + 1]->j = j;
				}
			}
		}
		// South East
		if (this->isValid(i + 1, j + 1))
		{
			if (isDestination(i + 1, j + 1, *endNode))
			{
				mapInfo[(i + 1) * this->Col + j + 1]->i = i;
				mapInfo[(i + 1) * this->Col + j + 1]->j = j;
				printf("The destination cell is found\n");
				this->shortestPath = tracePath(*endNode);
				this->destFound = true;
				return true;
			}
			else if (closedList[i + 1][j + 1] == false && isUnblocked(i + 1, j + 1) == true)
			{
				newG = mapInfo[i * this->Col + j]->g + 1.0f;
				newH = calculateH(i + 1, j + 1, *endNode);
				newF = newG + newH;

				if (mapInfo[(i + 1) * this->Col + j + 1]->f == FLT_MAX || mapInfo[(i + 1) * this->Col + j + 1]->f > newF)
				{
					cNode inNode;
					inNode.i = i + 1;
					inNode.j = j + 1;
					distanceList.push_back(std::make_pair(newF, inNode));
					mapInfo[(i + 1) * this->Col + j + 1]->f = newF;
					mapInfo[(i + 1) * this->Col + j + 1]->g = newG;
					mapInfo[(i + 1) * this->Col + j + 1]->h = newH;
					mapInfo[(i + 1) * this->Col + j + 1]->i = i;
					mapInfo[(i + 1) * this->Col + j + 1]->j = j;
				}
			}
		}
		// SouthWest
		if (this->isValid(i + 1, j - 1))
		{
			if (isDestination(i + 1, j - 1, *endNode))
			{
				mapInfo[(i + 1)* this->Col + j - 1]->i = i;
				mapInfo[(i + 1)* this->Col + j - 1]->j = j;
				printf("The destination cell is found\n");
				this->shortestPath = tracePath(*endNode);
				this->destFound = true;
				return true;
			}
			else if (closedList[i + 1][j - 1] == false && isUnblocked(i + 1, j - 1) == true)
			{
				newG = mapInfo[i * this->Col + j]->g + 1.0f;
				newH = calculateH(i + 1, j - 1, *endNode);
				newF = newG + newH;

				if (mapInfo[(i + 1)* this->Col + j - 1]->f == FLT_MAX || mapInfo[(i + 1)* this->Col + j - 1]->f > newF)
				{
					cNode inNode;
					inNode.i = i + 1;
					inNode.j = j - 1;
					distanceList.push_back(std::make_pair(newF, inNode));
					mapInfo[(i + 1)* this->Col + (j - 1)]->f = newF;
					mapInfo[(i + 1)* this->Col + (j - 1)]->g = newG;
					mapInfo[(i + 1)* this->Col + (j - 1)]->h = newH;
					mapInfo[(i + 1)* this->Col + (j - 1)]->i = i;
					mapInfo[(i + 1)* this->Col + (j - 1)]->j = j;
				}
			}
		}
		// North West
		if (this->isValid(i - 1, j - 1))
		{
			if (isDestination(i - 1, j - 1, *endNode))
			{
				mapInfo[(i - 1)* this->Col + j - 1]->i = i;
				mapInfo[(i - 1)* this->Col + j - 1]->j = j;
				printf("The destination cell is found\n");
				this->shortestPath = tracePath(*endNode);
				this->destFound = true;
				return true;
			}
			else if (closedList[i - 1][j - 1] == false && isUnblocked(i - 1, j - 1) == true)
			{
				newG = mapInfo[i * this->Col + j]->g + 1.0f;
				newH = calculateH(i - 1, j - 1, *endNode);
				newF = newG + newH;

				if (mapInfo[(i - 1)* this->Col + j - 1]->f == FLT_MAX || mapInfo[(i - 1)* this->Col + j - 1]->f > newF)
				{
					cNode inNode;
					inNode.i = i - 1;
					inNode.j = j - 1;
					distanceList.push_back(std::make_pair(newF, inNode));
					mapInfo[(i - 1)* this->Col + (j - 1)]->f = newF;
					mapInfo[(i - 1)* this->Col + (j - 1)]->g = newG;
					mapInfo[(i - 1)* this->Col + (j - 1)]->h = newH;
					mapInfo[(i - 1)* this->Col + (j - 1)]->i = i;
					mapInfo[(i - 1)* this->Col + (j - 1)]->j = j;
				}
			}
		}
	}

	if (this->destFound == false && doIter == true)
		printf("Failed to find the destination.\n");
	return false;
	return true;
}

bool AStar::isValid(int row, int col)
{
	return (row >= 0) && (row < this->Row) && (col >= 0) && (col < this->Col);
}
bool AStar::isUnblocked(int row, int col)
{
	if (this->Map[row][col] == 1)
		return true;
	else
		return false;
}
bool AStar::isDestination(int row, int col, cNode dest)
{
	if (row == dest.i && col == dest.j)
		return true;
	else
		return false;
}
bool AStar::calculateH(int row, int col, cNode dest)
{
	return ((double)sqrt((row - dest.i)*(row - dest.i) + (col - dest.j)*(col - dest.j)));
}
std::vector<cNode*> AStar::tracePath(cNode dest)
{
	printf("\nThe path is ");
	int row = dest.i;
	int col = dest.j;

	std::vector<cNode*> path;
	while (!(mapInfo[row * this->Col + col]->i == row && mapInfo[row * this->Col + col]->j == col))
	{
		cNode *curNode = new cNode;
		curNode->i = row;
		curNode->j = col;
		path.push_back(curNode);

		int tempRow = mapInfo[row * this->Col + col]->i;
		int tempCol = mapInfo[row * this->Col + col]->j;
		row = tempRow;
		col = tempCol;
	}

	cNode *curNode = new cNode;
	curNode->i = row;
	curNode->j = col;
	path.push_back(curNode);

	std::reverse(path.begin(), path.end());
	for (int i = 0; i != path.size(); i++)
	{
		printf("-> (%d,%d) ", path[i]->i, path[i]->j);
	}
	printf("\n");
	return path;
}