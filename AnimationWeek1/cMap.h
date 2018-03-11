#ifndef _MAP_HG_
#define _MAP_HG_

#include <vector>
#include <glad/glad.h>

#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "cGameObject.h"
#include "AStar.h"

class cMap
{
public:
	std::vector<cGameObject*> Nodes;
	std::vector<cGameObject*> ShortestPath;
	cMap();

	void Load(const GLchar* file);
	void Draw(cShader shader);

	AStar* aStar;
	int Col;
	int Row;
private:
	void init(std::vector<int> tileData);
};

#endif