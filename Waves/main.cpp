#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

const GLfloat WV = 5;

const float toRadians = 3.14159265f / 180.0f;
const int L=100;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

const int numIndices = 6 * (L - 1)*(L - 1);
unsigned int indices[numIndices];
GLfloat vertices[3 * L*L];

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void CreatePoints(GLfloat vertices[], unsigned int indices[]) {
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++) {
			vertices[3*(i*L + j)] = j;
			vertices[3*(i*L + j) + 1] = 0;
			vertices[3*(i*L + j) + 2] = i;
		}
	}

	for (int i = 0; i < L-1; i++) {
		for (int j = 0; j < L-1; j++) {
			indices[6 * (i*(L-1) + j)] = i * L + j;
			indices[6 * (i*(L-1) + j) + 1] = (i + 1)*L + j;
			indices[6 * (i*(L-1) + j) + 2] = (i + 1)*L + j + 1;

			indices[6 * (i*(L-1) + j) + 3] = i * L + j;
			indices[6 * (i*(L-1) + j) + 4] = (i + 1)*L + j + 1;
			indices[6 * (i*(L-1) + j) + 5] = i * L + j + 1;
		}
	}
}

void WaveMovement(GLfloat vertices[]) {
	for (size_t i = 0; i < L; i++) {
		for (size_t j = 0; j < L; j++) {
			vertices[3*(i*L + j)+1] =2 * sin(WV*glfwGetTime() + 0.4*i);
		}
	}
}

void CreateObjects() 
{
	CreatePoints(vertices, indices);
	WaveMovement(vertices);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 3*L*L, numIndices);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 3*L*L, numIndices);
	meshList.push_back(obj2);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main() 
{
	
	mainWindow = Window(1800, 1000);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 15.0f, 0.5f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
		deltaTime = now - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
		lastTime = now;

		// Get + Handle User Input
		glfwPollEvents();

		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);//

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();

		glm::mat4 model(1.0f);	
		WaveMovement(vertices);
		
		//meshList[0]->CreateMesh(vertices, indices, 3 * L*L, numIndices);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[0]->RenderMesh(vertices,3*L*L);
		/*
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();
		*/
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

