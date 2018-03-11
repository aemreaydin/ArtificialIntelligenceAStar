#include "cGameObject.h"
#include "cShader.h"


cGameObject::cGameObject(std::string modelName, std::string modelDir)
{
	this->Model1 = new cModel(modelDir.c_str());

	this->Position = glm::vec3(0.0f);
	this->Scale = glm::vec3(1.0f);
	this->OrientationQuat = glm::quat(0.0f, 0.0f, 0.0f, 1.0f);
	this->OrientationEuler = glm::vec3(0.0f);
}
cGameObject::cGameObject(std::string modelName, std::string modelDir, glm::vec3 position, glm::vec3 scale, glm::vec3 orientationEuler)
{
	this->Model1 = new cModel(modelDir.c_str());

	this->inOpenList = false;
	this->inShortestPath = false;

	this->Position = position;
	this->Scale = scale;
	this->OrientationQuat = glm::quat(orientationEuler);
	this->OrientationEuler = orientationEuler;
}

cGameObject::cGameObject(std::string modelName, std::string model1Dir, std::string model2Dir, glm::vec3 position, glm::vec3 scale, glm::vec3 orientationEuler)
{
	this->Model1 = new cModel(model1Dir.c_str());
	this->Model2 = new cModel(model2Dir.c_str());
	this->inShortestPath = false;

	this->Position = position;
	this->Scale = scale;
	this->OrientationQuat = glm::quat(orientationEuler);
	this->OrientationEuler = orientationEuler;
}
cGameObject::cGameObject(std::string modelName, std::string model1Dir, std::string model2Dir, std::string model3Dir, glm::vec3 position, glm::vec3 scale, glm::vec3 orientationEuler)
{
	this->Model1 = new cModel(model1Dir.c_str());
	this->Model2 = new cModel(model2Dir.c_str());
	this->Model3 = new cModel(model3Dir.c_str());

	this->inOpenList = false;
	this->inShortestPath = false;

	this->Position = position;
	this->Scale = scale;
	this->OrientationQuat = glm::quat(orientationEuler);
	this->OrientationEuler = orientationEuler;
}
void cGameObject::Draw(cShader Shader)
{
	if (!this->inShortestPath && !this->inOpenList)
		this->Model1->Draw(Shader);
	else if (this->inOpenList)
		this->Model2->Draw(Shader);
	else if(this->inShortestPath)
		this->Model2->Draw(Shader);
}