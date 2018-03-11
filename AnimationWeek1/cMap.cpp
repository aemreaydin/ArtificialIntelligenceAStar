#include "cMap.h"

#include <fstream>
#include <sstream>

cMap::cMap() { 
	this->aStar = new AStar;
	return;
}

void cMap::Load(const GLchar* file)
{
	this->Nodes.clear();
	GLuint tileCode;
	cMap map;
	std::string line;
	std::ifstream fstream(file);
	std::vector<int> tileData;

	if (fstream)
	{
		std::string temp;
		fstream >> temp;
		fstream >> this->Row;
		fstream >> temp;
		fstream >> this->Col;
		fstream >> temp;
		fstream >> aStar->startNode->i;
		fstream >> aStar->startNode->j;
		fstream >> temp;
		fstream >> aStar->endNode->i;
		fstream >> aStar->endNode->j;
		

		while (std::getline(fstream, line))
		{
			std::istringstream sstream(line);
			std::vector<GLuint> row;
			while (sstream >> tileCode)
				tileData.push_back(tileCode);
		}
		if (tileData.size() > 0)
		{
			this->init(tileData);
		}
	}
	aStar->LoadMap(tileData, this->Row, this->Col);
}

void cMap::Draw(cShader shader)
{
	//if (aStar->distanceList.size() != 0)
	//{
	//	for (int i = 0; i != this->Nodes.size(); i++)
	//	{
	//		this->Nodes[i]->inOpenList = false;
	//	}
	//	for (int i = 0; i != aStar->distanceList.size(); i++)
	//	{
	//		this->Nodes[aStar->distanceList[i].second.i * this->Col + aStar->distanceList[i].second.j]->inOpenList = true;
	//	}
	//}
	if (aStar->shortestPath.size ()!= 0)
	{
		for (int i = 0; i != aStar->shortestPath.size(); i++)
		{
			this->Nodes[aStar->shortestPath[i]->i * this->Col + aStar->shortestPath[i]->j]->inShortestPath = true;
		}
	}
	else
	{
		for (int i = 0; i != Nodes.size(); i++)
		{
			this->Nodes[i]->inShortestPath = false;
		}
	}
	for (int i = 0; i != this->Nodes.size(); i++)
	{
		glm::mat4 model = glm::mat4(1.0f);	
		model = glm::translate(model, this->Nodes[i]->Position);
		model = glm::rotate(model, glm::radians(this->Nodes[i]->OrientationEuler.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(this->Nodes[i]->OrientationEuler.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(this->Nodes[i]->OrientationEuler.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, this->Nodes[i]->Scale);
		shader.SetMatrix4("model", model, true);
		this->Nodes[i]->Draw(shader);
	}
}

void cMap::init(std::vector<int> tileData)
{
	for (int i = 0; i != this->Row; i++)
	{
		for (int j = 0; j != this->Col; j++)
		{
			if (tileData[i * this->Col + j] == 1)
			{
				glm::vec3 pos(i*2.0f, 0.0f, j*2.0f);
				glm::vec3 scale(1.0f);
				glm::vec3 orientation(0.0f);
				cGameObject* curObj = new cGameObject("Block", "assets/models/CubeBrick.obj", "assets/models/CubeGreen.obj", pos, scale, orientation);
				curObj->inShortestPath = false;
				Nodes.push_back(curObj);
			}
			else
			{
				glm::vec3 pos(i*2.0f, 0.0f, j*2.0f);
				glm::vec3 scale(1.0f);
				glm::vec3 orientation(0.0f);
				cGameObject* curObj = new cGameObject("Warning", "assets/models/CubeWarning.obj", pos, scale, orientation);
				Nodes.push_back(curObj);
			}
		}
	}
}