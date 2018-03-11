#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "cGLCalls.h"
#include "Camera.h"
#include "cShader.h"
#include "cModel.h"
#include "cGameObject.h"
#include "cLightManager.h"
#include "cMap.h"
#include "AStar.h"


#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#include <gtest\gtest.h>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

cGLCalls* GLCalls;
cShader * Shader, * LampShader;
cGameObject * Nanosuit, * SanFran;
cLightManager * LightManager;
cMap * Map;
AStar* aStar;

std::vector< cGameObject * > GOVec;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

const unsigned int width = 1600;
const unsigned int height = 900;

Camera camera(glm::vec3(0.0f, 10.0f, 10.0f));
float lastX = width / 2.0f;
float lastY = height / 2.0f;
bool firstMouse = true;
bool is3Person = true;;
glm::vec3 cameraDist = glm::vec3(0.0f, 5.0f, -5.0f);
float camSpeed = 5.0f;

cGameObject* AI;
bool canMove = false;
int moveIndex = 0;
bool isMoving = false;
bool isTurning = false;
bool pathFound = false;
enum lastMove
{
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN
};

int currentLight = 0;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void MoveAI(float deltaTime);
void MoveLeft(float deltaTime);
void MoveRight(float deltaTime);
void MoveUp(float deltaTime);
void MoveDown(float deltaTime);

TEST(TC_INIT, InitializeGLFW)
{
	ASSERT_TRUE(GLCalls->Initialize() == GL_TRUE);
}
TEST(TC_CREATE_WINDOW, CreateGLWindow)
{
	ASSERT_TRUE(GLCalls->CreateGLWindow() == GL_TRUE);
}

int main(int argc, char **argv)
{
	GLCalls = new cGLCalls(width, height, "AssimpImport");

	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();	

	glfwSetFramebufferSizeCallback(GLCalls->GetWindow(), framebuffer_size_callback);
	glfwSetCursorPosCallback(GLCalls->GetWindow(), mouse_callback);
	glfwSetScrollCallback(GLCalls->GetWindow(), scroll_callback);
	glfwSetKeyCallback(GLCalls->GetWindow(), key_callback);
	//glfwSetInputMode(GLCalls->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);

	Shader = new cShader("assets/shaders/simpleVertex.glsl", "assets/shaders/simpleFragment.glsl");
	LampShader = new cShader("assets/shaders/lampShader.glsl", "assets/shaders/lampFragment.glsl");



	LightManager = new cLightManager();
	LightManager->CreateLights();
	LightManager->LoadLampsIntoShader(*LampShader);

	//LightManager->LoadLampsIntoShader(*Shader);

	Map = new cMap();
	Map->Load("assets/MapSmall.txt");
	int start_i = Map->aStar->startNode->i;
	int end_i = Map->aStar->startNode->j;
	//AI = new cGameObject("AI", "assets/models/SphereNoTexture.blend", Map->Nodes[start_i * Map->Col + end_i]->Position, glm::vec3(1.0f), glm::vec3(0.0f));
	AI = new cGameObject("AI", "assets/models/nanosuit/nanosuitSmall.obj", glm::vec3(Map->Nodes[start_i * Map->Col + end_i]->Position.x, 1.0f, Map->Nodes[start_i * Map->Col + end_i]->Position.z), glm::vec3(1.0f), glm::vec3(0.0f));


	while (!glfwWindowShouldClose(GLCalls->GetWindow()))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		processInput(GLCalls->GetWindow());

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Shader->Use();
		LampShader->Use();

		glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), (float)width / (float)height, 0.1f, 100.0f);
		glm::mat4 view;
		if(is3Person)
			view = glm::lookAt(AI->Position + cameraDist, AI->Position, glm::vec3(0.0f, 1.0f, 0.0f));
		else
			view = camera.GetViewMatrix();

		Shader->SetMatrix4("projection", projection, true);
		Shader->SetMatrix4("view", view, true);
		Shader->SetVector3f("eyePos", camera.GetPosition());
		LampShader->SetMatrix4("projection", projection, true);
		LampShader->SetMatrix4("view", view, true);
		LightManager->LoadLightsIntoShader(*Shader);
		for (int i = 0; i < LightManager->NumLights; i++)
		{
			glm::mat4 lightModel = glm::mat4(1.0f);
			lightModel = glm::translate(lightModel, LightManager->Lights[i].position);
			lightModel = glm::scale(lightModel, glm::vec3(0.2f));
			Shader->SetMatrix4("lightModel", lightModel, true);
		}
		//LightManager->DrawLightsIntoScene(*LampShader);

		// DRAW AI
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, AI->Position);
		model = glm::rotate(model, glm::radians(AI->OrientationEuler.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(AI->OrientationEuler.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(AI->OrientationEuler.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, AI->Scale);
		Shader->SetMatrix4("model", model, true);
		
		MoveAI(deltaTime);
		AI->Draw(*Shader);

		Map->Draw(*Shader);

		glfwPollEvents();
		glfwSwapBuffers(GLCalls->GetWindow());
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);


	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		pathFound = true;
		Map->aStar->doIter = true;
		Map->aStar->DoAStar();
		for (int i = 0; i != Map->aStar->shortestPath.size(); i++)
		{
			int start_i = Map->aStar->shortestPath[i]->i;
			int end_i = Map->aStar->shortestPath[i]->j;
			Map->ShortestPath.push_back(Map->Nodes[start_i * Map->Col + end_i]);
		}
	}
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
	{
		pathFound = true;
		Map->aStar->doIter = true;
		Map->aStar->DoAStar8Way();
		for (int i = 0; i != Map->aStar->shortestPath.size(); i++)
		{
			int start_i = Map->aStar->shortestPath[i]->i;
			int end_i = Map->aStar->shortestPath[i]->j;
			Map->ShortestPath.push_back(Map->Nodes[start_i * Map->Col + end_i]);
		}
	}
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		if(pathFound)
			canMove = true;
	}
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
		is3Person = !is3Person;
}

void processInput(GLFWwindow *window)
{
	if (!is3Person)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	else
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraDist.x += camSpeed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraDist.x -= camSpeed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraDist.z += camSpeed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraDist.z -= camSpeed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			cameraDist.y += camSpeed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			cameraDist.y -= camSpeed * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		AI->OrientationEuler.y += camSpeed * 5.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		AI->OrientationEuler.y -= camSpeed * 5.0f * deltaTime;

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void MoveAI(float deltaTime)
{
	std::vector<cNode*> vecNodes = Map->aStar->shortestPath;
	std::vector<cGameObject*> vecObjects = Map->ShortestPath;

	float distanceX, distanceZ;
	//lastMove last = MOVE_LEFT;
	if (canMove)
	{
		if (!isMoving)
		{
			moveIndex++;
			if (moveIndex == vecObjects.size())
			{
				moveIndex = 0;
				AI->Position = glm::vec3(vecObjects[moveIndex]->Position.x, 1.0f, vecObjects[moveIndex]->Position.x);
			}
		}
		if (AI->Position.x < vecObjects[moveIndex]->Position.x && glm::distance(AI->Position.z, vecObjects[moveIndex]->Position.z) <= 0.1f)
		{
			isMoving = true;

				MoveUp(deltaTime);
		}
		if (AI->Position.x > vecObjects[moveIndex]->Position.x && glm::distance(AI->Position.z, vecObjects[moveIndex]->Position.z) <= 0.1f)
		{
			isMoving = true;

				MoveDown(deltaTime);
		}
		if (glm::distance(AI->Position.x, vecObjects[moveIndex]->Position.x) <= 0.1f && AI->Position.z < vecObjects[moveIndex]->Position.z)
		{
			isMoving = true;

				MoveRight(deltaTime);
		}
		if (glm::distance(AI->Position.x, vecObjects[moveIndex]->Position.x) <= 0.1f  && AI->Position.z > vecObjects[moveIndex]->Position.z)
		{
			isMoving = true;

				MoveLeft(deltaTime);
		}
		if (AI->Position.x > vecObjects[moveIndex]->Position.x  && AI->Position.z > vecObjects[moveIndex]->Position.z)
		{
			isMoving = true;

			MoveDown(deltaTime);
			MoveLeft(deltaTime);
		}
		if (AI->Position.x > vecObjects[moveIndex]->Position.x  && AI->Position.z < vecObjects[moveIndex]->Position.z)
		{
			isMoving = true;

			MoveDown(deltaTime);
			MoveRight(deltaTime);
		}
		if (AI->Position.x < vecObjects[moveIndex]->Position.x  && AI->Position.z > vecObjects[moveIndex]->Position.z)
		{
			isMoving = true;

			MoveUp(deltaTime);
			MoveLeft(deltaTime);
		}
		if (AI->Position.x < vecObjects[moveIndex]->Position.x  && AI->Position.z < vecObjects[moveIndex]->Position.z)
		{
			isMoving = true;

			MoveUp(deltaTime);
			MoveRight(deltaTime);
		}
		distanceX = glm::distance(AI->Position.x, vecObjects[moveIndex]->Position.x);
		distanceZ = glm::distance(AI->Position.z, vecObjects[moveIndex]->Position.z);
		if (distanceX <= 0.1f && distanceZ <= 0.1f)
		{
			AI->Position.x = vecObjects[moveIndex]->Position.x;
			AI->Position.z = vecObjects[moveIndex]->Position.z;
			isMoving = false;
			canMove = false;
		}
		//AI->Position.x = vecObjects[moveIndex]->Position.x;
		//AI->Position.z = vecObjects[moveIndex]->Position.z;
		//canMove = false;
	}
}
void MoveLeft(float deltaTime)
{
	AI->Position.z -= 2.0f * deltaTime;
}
void MoveRight(float deltaTime)
{
	AI->Position.z += 2.0f * deltaTime;
}
void MoveUp(float deltaTime)
{
	AI->Position.x += 2.0f * deltaTime;
}
void MoveDown(float deltaTime)
{
	AI->Position.x -= 2.0f * deltaTime;
}