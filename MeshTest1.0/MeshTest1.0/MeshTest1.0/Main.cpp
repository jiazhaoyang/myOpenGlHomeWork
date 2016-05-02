#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <time.h>

#include "Shader.h"
#include "RenderObject.hpp"
#include "RayTest.h"

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

enum TerrainEnum {
	Terrain1,
	Terrain2,
	TerrainEnum_MAX
};

enum BuildingEnum {
	Building_NONE,
	Building1,
	Building2,
	BuildingEnum_MAX
};

struct Grid {
	TerrainEnum terrainType;
	BuildingEnum buildingType;
	int data;
};


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void cursor_pos_callback(GLFWwindow* window, double x, double y);
void mouse_button_callback(GLFWwindow* window, int mouse, int action, int mods);
void genTerrainRandom(Grid* grid, int length, int typeCount);
void drawTerrain();

const GLuint WIDTH = 1080, HEIGHT = 720;

GLuint mouseX, mouseY;
GLuint lastMouseX, lastMouseY;
GLFWwindow* window;
glm::vec3 cameraPosition(0.0f, 6.0f, 10.0f);
glm::vec3 cameraPositionSaved;
glm::vec3 ray_origin;
glm::vec3 ray_direction;
glm::mat4 projection;
glm::mat4 view;
glm::vec3 aabb_min(0.0f, 0.0f, -1.0f);
glm::vec3 aabb_max(1.0f, 0.0f, 0.0f);
bool rotateView = false;
bool leftButtonClick = false;

float intersection_distance; // Output of TestRayOBBIntersection()
Grid grid[10][10] = {};
std::map<int, RenderObject*> terrainDictionary;
std::map<int, RenderObject*> buildingDictionary;

void terrainInitial(){
	// ================================== terrain START ====================================== //
	GLfloat vertices[] = {
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, -1.0f, 0.0f, 1.0f
	};

	GLuint indeces[] = {
		0, 1, 2, 2, 3, 0
	};

	RenderObject* ter1 = new RenderObject(1, LoadShader("meshVertex.vert", "meshFragment.frag"));
	ter1->BindMesh_p3_t2(vertices, sizeof(vertices), indeces, sizeof(indeces));
	ter1->SetTexture("container.png");
	terrainDictionary.insert(std::map<TerrainEnum, RenderObject*>::value_type(Terrain1, ter1));

	RenderObject* ter2 = new RenderObject(2, LoadShader("meshVertex.vert", "meshFragment.frag"));
	ter2->BindMesh_p3_t2(vertices, sizeof(vertices), indeces, sizeof(indeces));
	ter2->SetTexture("awesomeface.png");
	terrainDictionary.insert(std::map<TerrainEnum, RenderObject*>::value_type(Terrain2, ter2));
	// ================================== terrain DONE ====================================== //
}

void buildingInitial(){

	// ================================== building START ====================================== //
	GLfloat vertices1[] = {
		0.5f, 0.0f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.2f, 1.0f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.9f, 1.0f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.9f, 1.0f, -0.9f, 1.0f, 1.0f, 1.0f, 1.0f
	};

	GLfloat vertices2[] = {
		0.5f, 0.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.2f, 1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.9f, 1.0f, 0.1f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.9f, 1.0f, -0.9f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	GLuint indeces1[] = {
		0, 1, 2, 0, 2, 3, 0, 3, 1, 1, 2, 3
	};

	RenderObject* build1 = new RenderObject(3, LoadShader("singlecolor.vert", "singlecolor.frag"));
	build1->BindMesh_p3_c4(vertices1, sizeof(vertices1), indeces1, sizeof(indeces1));
	buildingDictionary.insert(std::map<BuildingEnum, RenderObject*>::value_type(Building1, build1));

	RenderObject* build2 = new RenderObject(4, LoadShader("singlecolor.vert", "singlecolor.frag"));
	build2->BindMesh_p3_c4(vertices2, sizeof(vertices2), indeces1, sizeof(indeces1));
	buildingDictionary.insert(std::map<BuildingEnum, RenderObject*>::value_type(Building2, build2));
	// ================================== building DONE ====================================== //


}
// The MAIN function, from here we start the application and run the game loop
int main()
{
	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	 window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glewExperimental = GL_TRUE;
	glewInit();

	glViewport(0, 0, WIDTH, HEIGHT);

	glEnable(GL_DEPTH_TEST);
	
	projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

	terrainInitial();
	
	buildingInitial();



	// Generate terrain randomly
	genTerrainRandom(grid[0], sizeof(grid) / sizeof(grid[0][0]), TerrainEnum_MAX);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// ========================================= Draw Meshes From Array ========================================== //

		if (rotateView)
		{
			cameraPosition = glm::vec3(glm::rotate(glm::mat4(), -((GLint)mouseX - (GLint)lastMouseX) * 20.0f / WIDTH, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(cameraPositionSaved, 1.0f));
		}
		 view = glm::lookAt(
			cameraPosition,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
			);
		
		
		 ScreenPosToWorldRay(
			 mouseX, mouseY,
			 WIDTH, HEIGHT,
			 view,
			 projection,
			 ray_origin,
			 ray_direction
			 );

		drawTerrain();
		
	// ========================== Draw Done ================================= //


		glfwSwapBuffers(window);
	}
	
	for (std::map<int, RenderObject*>::iterator it = terrainDictionary.begin(); it != terrainDictionary.end(); it++){
		delete it->second;
	}

	for (std::map<int, RenderObject*>::iterator it = buildingDictionary.begin(); it != buildingDictionary.end(); it++){
		delete it->second;
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void cursor_pos_callback(GLFWwindow* window, double x, double y)
{
	mouseX = x;
	mouseY = HEIGHT - y;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
{
	if (button == GLFW_MOUSE_BUTTON_MIDDLE)
	{
		if (action == GLFW_PRESS) {
			rotateView = true;
			lastMouseX = mouseX;
			lastMouseY = mouseY;
			cameraPositionSaved = cameraPosition;
		}
		else if (action == GLFW_RELEASE)
			rotateView = false;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS) {
			leftButtonClick = true;
		}
		else if (action == GLFW_RELEASE)
			leftButtonClick = false;
	}
}


void genTerrainRandom(Grid* grid, int length, int typeCount)
{
	srand((int)time(0));
	for (int i = 0; i < length; i++)
	{
		grid[i].terrainType = (TerrainEnum)(rand() % typeCount);
	}
}

//====================draw the terrain=======================//
void drawTerrain(){
	// Grid Mesh Drawing LOOP
	RenderObject* curTerrain;
	RenderObject* curBuilding;
	Grid* curGrid;

	//	drawTerrain(curTerrain, curBuilding, curGrid);
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			curGrid = &grid[i][j];

			curTerrain = terrainDictionary.find(curGrid->terrainType)->second;


			glUseProgram(curTerrain->shaderProgram);
			curTerrain->SetPosition(i - 5, 0, 5 - j);

			if (TestRayOBBIntersection(
				ray_origin,
				ray_direction,
				aabb_min,
				aabb_max,
				curTerrain->model,
				intersection_distance)
				)
			{
				glUniform1i(glGetUniformLocation(curTerrain->shaderProgram, "_Pick"), 1);

				if (leftButtonClick)
				{

					switch (curGrid->terrainType){

					case Terrain1:
						curGrid->buildingType = Building1;
						break;
					case Terrain2:
						curGrid->buildingType = Building2;

					default:
						break;


					}
				}
			}
			else{
				glUniform1i(glGetUniformLocation(curTerrain->shaderProgram, "_Pick"), 0);
			}
			curTerrain->Draw(&view, &projection);

			if (curGrid->buildingType != Building_NONE)
			{
				curBuilding = buildingDictionary.find(curGrid->buildingType)->second;

				glUseProgram(curBuilding->shaderProgram);
				curBuilding->SetPosition(i - 5, 0, 5 - j);
				curBuilding->Draw(&view, &projection);
			}

		}
	}
}
