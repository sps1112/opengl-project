// Library headers
#include <glad/glad.h>	// first glad
#include <GLFW/glfw3.h> // then glfw
#include <stb_image.h>	// for loading images

// glm libraries for math
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

// Custom Headers
#include <Shader.h>		// shader header
#include <Camera.h>		// camera header
#include <Model.h>		// model header
#include <FileSystem.h> // Filesystem header
#include <Utils.h>		// Utility header
#include <Primitive.h>	// Primitive header

#include <iostream>
#include <vector>

// function declarations
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void processColor(GLFWwindow *window);
void processDraw(GLFWwindow *window);
float processSlider(GLFWwindow *window, float sliderValue);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// Const settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Camera settings
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool isFirstMouse = true;

// Frame settings
float lastFrameTime = 0.0f;
float deltaTime = 0.0f;

// main function
int main()
{
	// glfw initialise
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);				   // open gl version 3.x
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);				   // version is 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // use core profile
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);		   // for Mac

	// Window intialize
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Window", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);									   // current window
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // callback set for size change
	glfwSetCursorPosCallback(window, mouse_callback);				   // callback for mouse movement
	glfwSetScrollCallback(window, scroll_callback);					   // callback for zoom

	// check GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to intialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST); // Enable Z buffering
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);// disable cursor
	stbi_set_flip_vertically_on_load(true); // set before loading model

	// Vertex Data
	// Cube vertices with texture
	/*
	float vertices[] = {
		// vertices            // color         // texture UV
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f};
	*/

	// World POS for each instance of cube
	/*
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)};
	*/
	/*
	float vertices[] = {
		// vertices         // normal
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f};
		*/

	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, -2.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)};

	/*

	// glm::vec3 lightPos(2.5f, 0.0f, -1.5f);
	glm::vec3 lightPos(1.5f, 0.25f, -2.0f);

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f, 0.2f, 2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f, 2.0f, -12.0f),
		glm::vec3(0.0f, 0.0f, -3.0f)};
		*/
	/*
	// GPU Memory Setup
	// initialise variables
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO); // VAO stores vertex attributes
	glGenBuffers(1, &VBO);		// VBO stores vertex data in bulk
	glGenBuffers(1, &EBO);		// EBO stores indices of vertices
	// Bind Local variables
	// Link VAO
	glBindVertexArray(VAO); // Link Vertex Array to VAO
	// Link VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);										   //Link Current Array Buffer to VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //Map vertices to Array Buffer i.e. VBO
	*/

	/*
	//Link EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);//Link Current Element Array Buffer to EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);//Map Vertex Indices to Element Array Buffer i.e. EBO*/

	//Setup Attributes
	//layout location,elements in each attribute,data type,to normalize,stride gap,start index
	/*
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//format of vertices to be read
	glEnableVertexAttribArray(0);//enables Array
	*/

	/*
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	*/

	/*
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Setup VAO for the light source
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	// Clear Memory
	// first VBO then VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind array buffer
	glBindVertexArray(0);			  // unbind vertex Array

	*/

	// Model ourModel(FileSystem::getPath("resources/models/backpack/backpack.obj"));
	// Model ourModel(FileSystem::getPath("resources/models/sphere/sphere.obj"));
	Primitive primitive1(FileSystem::getPath("resources/primitives/2D/triangle.2d").c_str());
	Primitive primitive2(FileSystem::getPath("resources/primitives/3D/cube.3d").c_str());
	// primitive2.PrintVertex();
	Log("data setup finished");

	// Create shader
	// Standard Shader
	Shader mainShader(FileSystem::getPath("shaders/shader.vs").c_str(), FileSystem::getPath("shaders/shader.fs").c_str());

	// Lighted Object Shader
	Shader lightingShader(FileSystem::getPath("shaders/shader_light.vs").c_str(), FileSystem::getPath("shaders/shader_light.fs").c_str());

	// Gouraud Shader
	Shader gouraudShader(FileSystem::getPath("shaders/shader_gouraud.vs").c_str(), FileSystem::getPath("shaders/shader_gouraud.fs").c_str());

	// Light Source Shader
	Shader sourceShader(FileSystem::getPath("shaders/shader_source.vs").c_str(), FileSystem::getPath("shaders/shader_source.fs").c_str());

	// Material Shader
	Shader materialShader(FileSystem::getPath("shaders/shader_material.vs").c_str(), FileSystem::getPath("shaders/shader_material.fs").c_str());

	Shader shader2D(FileSystem::getPath("shaders/shader_2d.vs").c_str(), FileSystem::getPath("shaders/shader_2d.fs").c_str());
	Shader modelShader(FileSystem::getPath("shaders/shader_3d.vs").c_str(), FileSystem::getPath("shaders/shader_3d.fs").c_str());
	Shader shader3D(FileSystem::getPath("shaders/shader_3d.vs").c_str(), FileSystem::getPath("shaders/shader_3d_col.fs").c_str());

	// Texture Shader
	// Shader textureShader("../shaders/shader_texture.vs", "../shaders/shader_texture.fs");
	// Shader textureShader("../shaders/shader_texture.vs", "../shaders/shader_directional.fs");
	// Shader textureShader("../shaders/shader_texture.vs", "../shaders/shader_point.fs");
	// Shader textureShader("../shaders/shader_texture.vs", "../shaders/shader_spotlight.fs");
	// Shader textureShader("../shaders/shader_texture.vs", "../shaders/shader_scene.fs");

	// Model Shader
	// Shader modelShader(FileSystem::getPath("shaders/shader_model.vs").c_str(), FileSystem::getPath("shaders/shader_model.fs").c_str());
	Log("shader data setup");

	/*vector<pTexture> textures;
	pTexture mainTexture;
	mainTexture.id = LoadTexture(FileSystem::getPath("resources/textures/awesomeface.png").c_str());
	Log("Texture loaded");
	mainTexture.type = "texture_diffuse";
	textures.push_back(mainTexture);
	primitive1.SetupTextures(textures);*/

	vector<pTexture> textures3D;
	pTexture diffuse, specular, emmision;
	diffuse.id = LoadTexture(FileSystem::getPath("resources/textures/container2.png").c_str());
	diffuse.type = "texture_diffuse";
	textures3D.push_back(diffuse);
	specular.id = LoadTexture(FileSystem::getPath("resources/textures/container2_specular.png").c_str());
	specular.type = "texture_specular";
	textures3D.push_back(specular);
	emmision.id = LoadTexture(FileSystem::getPath("resources/textures/matrix.jpg").c_str());
	emmision.type = "texture_emmision";
	textures3D.push_back(emmision);
	primitive2.SetupTextures(textures3D);

	/*
	// Texture setup
	unsigned int texture, texture2;
	texture = LoadTexture(FileSystem::getPath("resources/textures /container.jpg").c_str());
	texture2 = LoadTexture(FileSystem::getPath("resources/textures/awesomeface.png").c_str());
	unsigned int diffusionMap, specularMap, specularMapColor, emmisionMap;
	diffusionMap = LoadTexture(FileSystem::getPath("resources/textures/container2.png").c_str());
	specularMap = LoadTexture(FileSystem::getPath("resources/textures/container2_specular.png").c_str());
	specularMapColor = LoadTexture(FileSystem::getPath("resources/textures/container2_specular_color.png").c_str());
	emmisionMap = LoadTexture(FileSystem::getPath("resources/textures/matrix.jpg").c_str());
	mainShader.use();
	// mainShader.setInt("texture1", 0);
	// mainShader.setInt("texture2", 1);
	textureShader.use();
	textureShader.setInt("material.diffuse", 0);
	textureShader.setInt("material.specular", 1);
	// textureShader.setInt("material.specular", 2);
	textureShader.setInt("material.emmision", 3);
	*/

	// Values setup
	float sliderValue = 0.5f;
	float timePeriod = 3.0f;
	// int gapLoc = glGetUniformLocation(mainShader.ID, "timeGap");
	// int sliderLoc = glGetUniformLocation(mainShader.ID, "slider");
	// mainShader.setFloat("timePeriod", timePeriod);
	float timeGap = 0.0f;
	float angleVal = 0.0f;

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f, 0.2f, 2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f, 2.0f, -12.0f),
		glm::vec3(0.0f, 0.0f, -3.0f)};

	unsigned int lightVAO, VBO, EBO;
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(lightVAO);
	// Link VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);										   //Link Current Array Buffer to VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //Map vertices to Array Buffer i.e. VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind array buffer
	glBindVertexArray(0);			  // unbind vertex Array
	Log("Setup finished");

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// delta time refresh
		float currentFrameTime = glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;
		timeGap += deltaTime;
		if (timeGap >= timePeriod)
		{
			timeGap -= timePeriod;
		}

		// input commands
		processInput(window);
		processColor(window);
		processDraw(window);

		// bind textures

		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D, texture);
		// glActiveTexture(GL_TEXTURE1);
		// glBindTexture(GL_TEXTURE_2D, texture2);

		/*
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffusionMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, specularMapColor);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, emmisionMap);
		*/

		/*
		// use Shader
		mainShader.use();
		sliderValue = processSlider(window, sliderValue);
		glUniform1f(sliderLoc, sliderValue);
		glUniform1f(gapLoc, timeGap);
		*/

		// view matrix :: WORLD TO VIEW
		glm::mat4 view;
		view = camera.GetViewMatrix();
		mainShader.setMat4("view", view);

		// projection matrix:: VIEW TO CLIPPED
		glm::mat4 projection;
		/*
		FOV = 45 degrees
		RATIO = HEIGHT/WIDTH
		Near Plane = 0.1
		Far Plane = 100.0
		*/
		projection = glm::perspective(glm::radians(camera.Zoom), ((float)SCR_WIDTH) / ((float)SCR_HEIGHT), 0.1f, 100.0f);
		// mainShader.setMat4("projection", projection);

		glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
		glm::mat4 lightModel = glm::mat4(1.0f);
		if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
		{
			angleVal += deltaTime * (360.0f / timePeriod);
			if (angleVal > 360)
			{
				angleVal -= 360;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
		{
			angleVal -= deltaTime * (360.0f / timePeriod);
			if (angleVal < 0)
			{
				angleVal += 360;
			}
		}
		lightModel = glm::rotate(lightModel, glm::radians(angleVal), glm::vec3(0.0f, 1.0f, 0.0f));

		sourceShader.use();
		sourceShader.setMat4("view", view);
		sourceShader.setMat4("projection", projection);
		sourceShader.setVec3("sourceColor", lightColor);
		glBindVertexArray(lightVAO);
		for (int i = 0; i < 4; i++)
		{
			glm::mat4 model = lightModel;
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			sourceShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glm::vec3 objectColor(1.0f, 0.5f, 0.31f);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.0f));

		modelShader.use();
		modelShader.setMat4("projection", projection);
		modelShader.setMat4("view", view);
		modelShader.setMat4("model", model);
		modelShader.setVec3("viewPos", camera.Position);
		modelShader.setFloat("material.shininess", 64);

		modelShader.setVec3("dirLight.direction", glm::vec3(0.5f, -1.0f, -0.7f));
		modelShader.setVec3("dirLight.ambient", lightColor * 0.1f);
		modelShader.setVec3("dirLight.diffuse", lightColor * 0.25f);
		modelShader.setVec3("dirLight.specular", glm::vec3(0.5f));

		lightModel = glm::mat4(1.0f);
		lightModel = glm::rotate(lightModel, glm::radians(angleVal), glm::vec3(0.0f, 1.0f, 0.0f));
		lightModel = glm::translate(lightModel, pointLightPositions[0]);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));
		glm::vec3 newLightPos = (lightModel * glm::vec4(1.0f));

		modelShader.setVec3("pointLights[0].position", newLightPos);
		modelShader.setFloat("pointLights[0].constant", 1.0f);
		modelShader.setFloat("pointLights[0].linear", 0.22f);
		modelShader.setFloat("pointLights[0].quadratic", 0.20f);
		modelShader.setVec3("pointLights[0].ambient", lightColor * 0.05f);
		modelShader.setVec3("pointLights[0].diffuse", lightColor * 0.4f);
		modelShader.setVec3("pointLights[0].specular", glm::vec3(1.0f));

		lightModel = glm::mat4(1.0f);
		lightModel = glm::rotate(lightModel, glm::radians(angleVal), glm::vec3(0.0f, 1.0f, 0.0f));
		lightModel = glm::translate(lightModel, pointLightPositions[0]);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));
		newLightPos = (lightModel * glm::vec4(1.0f));

		modelShader.setVec3("pointLights[1].position", newLightPos);
		modelShader.setFloat("pointLights[1].constant", 1.0f);
		modelShader.setFloat("pointLights[1].linear", 0.22f);
		modelShader.setFloat("pointLights[1].quadratic", 0.20f);
		modelShader.setVec3("pointLights[1].ambient", lightColor * 0.05f);
		modelShader.setVec3("pointLights[1].diffuse", lightColor * 0.4f);
		modelShader.setVec3("pointLights[1].specular", glm::vec3(0.5f));

		lightModel = glm::mat4(1.0f);
		lightModel = glm::rotate(lightModel, glm::radians(angleVal), glm::vec3(0.0f, 1.0f, 0.0f));
		lightModel = glm::translate(lightModel, pointLightPositions[0]);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));
		newLightPos = (lightModel * glm::vec4(1.0f));

		modelShader.setVec3("pointLights[2].position", newLightPos);
		modelShader.setFloat("pointLights[2].constant", 1.0f);
		modelShader.setFloat("pointLights[2].linear", 0.22f);
		modelShader.setFloat("pointLights[2].quadratic", 0.20f);
		modelShader.setVec3("pointLights[2].ambient", lightColor * 0.05f);
		modelShader.setVec3("pointLights[2].diffuse", lightColor * 0.4f);
		modelShader.setVec3("pointLights[2].specular", glm::vec3(0.5f));

		lightModel = glm::mat4(1.0f);
		lightModel = glm::rotate(lightModel, glm::radians(angleVal), glm::vec3(0.0f, 1.0f, 0.0f));
		lightModel = glm::translate(lightModel, pointLightPositions[0]);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));
		newLightPos = (lightModel * glm::vec4(1.0f));

		modelShader.setVec3("pointLights[3].position", newLightPos);
		modelShader.setFloat("pointLights[3].constant", 1.0f);
		modelShader.setFloat("pointLights[3].linear", 0.22f);
		modelShader.setFloat("pointLights[3].quadratic", 0.20f);
		modelShader.setVec3("pointLights[3].ambient", lightColor * 0.05f);
		modelShader.setVec3("pointLights[3].diffuse", lightColor * 0.4f);
		modelShader.setVec3("pointLights[3].specular", glm::vec3(0.5f));

		modelShader.setVec3("spotLight.position", camera.Position);
		modelShader.setVec3("spotLight.direction", camera.Front);
		modelShader.setFloat("spotLight.cutoff", glm::cos(glm::radians(12.5f)));
		modelShader.setFloat("spotLight.outerCutoff", glm::cos(glm::radians(20.0f)));
		modelShader.setVec3("spotLight.ambient", lightColor * 0.05f);
		modelShader.setVec3("spotLight.diffuse", lightColor * 0.3f);
		modelShader.setVec3("spotLight.specular", glm::vec3(0.5f));

		// ourModel.Draw(modelShader);

		// shader2D.use();
		// primitive1.Draw(shader2D);
		/*glm::mat4 model3D(1.0f);
		model3D = glm::translate(model3D, glm::vec3(0.0f, -1.0f, -2.0f));
		shader3D.use();
		shader3D.setMat4("model", model3D);
		shader3D.setMat4("view", view);
		shader3D.setMat4("projection", projection);
		shader3D.setVec3("viewPos", camera.Position);
		shader3D.setVec3("material.ambient", objectColor);
		shader3D.setVec3("material.diffuse", objectColor);
		shader3D.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		shader3D.setFloat("material.shininess", 64);
		shader3D.setVec3("light.ambient", lightColor * 0.2f);
		shader3D.setVec3("light.diffuse", lightColor * 0.5f);
		shader3D.setVec3("light.specular", glm::vec3(1.0f));
		shader3D.setVec3("light.position", newLightPos);*/
		primitive2.Draw(modelShader);

		/*

		// Draw Light Source
		glm::mat4 lightModel = glm::mat4(1.0f);
		// float angleVal = 360.0f * timeGap / timePeriod;
		if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
		{
			angleVal += deltaTime * (360.0f / timePeriod);
			if (angleVal > 360)
			{
				angleVal -= 360;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
		{
			angleVal -= deltaTime * (360.0f / timePeriod);
			if (angleVal < 0)
			{
				angleVal += 360;
			}
		}
		lightModel = glm::rotate(lightModel, glm::radians(angleVal), glm::vec3(0.0f, 1.0f, 0.0f));
		*/

		/*lightModel = glm::translate(lightModel, lightPos);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));*/

		/*glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
		sourceShader.use();
		sourceShader.setMat4("view", view);
		sourceShader.setMat4("projection", projection);
		sourceShader.setVec3("sourceColor", lightColor);
		glBindVertexArray(lightVAO);
		for (int i = 0; i < 4; i++)
		{
			glm::mat4 model = lightModel;
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			sourceShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
		glm::vec3 newLightPos(lightModel * glm::vec4(1.0f));
		*/

		if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
		{
			/*// Use Gouraud Shader
			gouraudShader.use();
			gouraudShader.setVec3("lightColor", lightColor);
			gouraudShader.setVec3("objectColor", objectColor);
			gouraudShader.setVec3("lightPos", newLightPos);
			gouraudShader.setVec3("viewPos", camera.Position);
			gouraudShader.setMat4("view", view);
			gouraudShader.setMat4("projection", projection);*/
			// Use Lighting Shader
			/*lightingShader.use();
			lightingShader.setVec3("objectColor", objectColor);
			lightingShader.setVec3("lightColor", lightColor);
			lightingShader.setVec3("lightPos", newLightPos);
			lightingShader.setVec3("viewPos", camera.Position);
			lightingShader.setMat4("view", view);
			lightingShader.setMat4("projection", projection);*/
		}
		/*	else if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS)
		{
			// Use Material Shader
			materialShader.use();
			materialShader.setVec3("objectColor", objectColor);
			materialShader.setVec3("lightColor", lightColor);
			materialShader.setVec3("lightPos", newLightPos);
			materialShader.setVec3("viewPos", camera.Position);
			materialShader.setMat4("view", view);
			materialShader.setMat4("projection", projection);
			materialShader.setVec3("material.ambient", objectColor);
			materialShader.setVec3("material.diffuse", objectColor);
			materialShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
			materialShader.setFloat("material.shininess", 64);
			materialShader.setVec3("light.ambient", lightColor * 0.2f);
			materialShader.setVec3("light.diffuse", lightColor * 0.5f);
			materialShader.setVec3("light.specular", glm::vec3(1.0f));
			materialShader.setVec3("light.position", newLightPos);
		}*/
		else
		{
			/*// Use Material Shader
			textureShader.use();
			textureShader.setVec3("viewPos", camera.Position);
			textureShader.setMat4("view", view);
			textureShader.setMat4("projection", projection);
			textureShader.setVec3("material.ambient", objectColor);
			textureShader.setVec3("material.diffuse", objectColor);
			textureShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
			textureShader.setFloat("material.shininess", 32);*/

			/*
			// Single Light System
			textureShader.setVec3("light.ambient", lightColor * 0.2f);
			textureShader.setVec3("light.diffuse", lightColor * 0.5f);
			textureShader.setVec3("light.specular", glm::vec3(1.0f));
			*/

			// Point Light
			/*
				textureShader.setVec3("light.position", newLightPos);
				if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
				{
					textureShader.setBool("check", true);
				}
				else
				{
					textureShader.setBool("check", false);
				}
				textureShader.setFloat("light.constant", 1.0f);
				textureShader.setFloat("light.linear", 0.22f);
				textureShader.setFloat("light.quadratic", 0.20f);
				*/

			// Directional Light
			/*
				textureShader.setVec3("light.direction", glm::vec3(0.0f, -0.0f, -1.0f));
				*/

			// SpotLight
			/*
			textureShader.setVec3("light.position", camera.Position);
			textureShader.setVec3("light.direction", camera.Front);
			textureShader.setFloat("light.cutoff", glm::cos(glm::radians(12.5f)));
			textureShader.setFloat("light.outerCutoff", glm::cos(glm::radians(20.0f)));
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			{
				textureShader.setBool("check", true);
			}
			else
			{
				textureShader.setBool("check", false);
			}
			*/
			/*
			// Multi Light System
			textureShader.setVec3("dirLight.direction", glm::vec3(0.5f, -1.0f, -0.7f));
			textureShader.setVec3("dirLight.ambient", lightColor * 0.1f);
			textureShader.setVec3("dirLight.diffuse", lightColor * 0.25f);
			textureShader.setVec3("dirLight.specular", glm::vec3(0.5f));

			lightModel = glm::mat4(1.0f);
			lightModel = glm::rotate(lightModel, glm::radians(angleVal), glm::vec3(0.0f, 1.0f, 0.0f));
			lightModel = glm::translate(lightModel, pointLightPositions[0]);
			lightModel = glm::scale(lightModel, glm::vec3(0.2f));
			newLightPos = (lightModel * glm::vec4(1.0f));

			textureShader.setVec3("pointLights[0].position", newLightPos);
			textureShader.setFloat("pointLights[0].constant", 1.0f);
			textureShader.setFloat("pointLights[0].linear", 0.22f);
			textureShader.setFloat("pointLights[0].quadratic", 0.20f);
			textureShader.setVec3("pointLights[0].ambient", lightColor * 0.05f);
			textureShader.setVec3("pointLights[0].diffuse", lightColor * 0.4f);
			textureShader.setVec3("pointLights[0].specular", glm::vec3(1.0f));

			lightModel = glm::mat4(1.0f);
			lightModel = glm::rotate(lightModel, glm::radians(angleVal), glm::vec3(0.0f, 1.0f, 0.0f));
			lightModel = glm::translate(lightModel, pointLightPositions[0]);
			lightModel = glm::scale(lightModel, glm::vec3(0.2f));
			newLightPos = (lightModel * glm::vec4(1.0f));

			textureShader.setVec3("pointLights[1].position", newLightPos);
			textureShader.setFloat("pointLights[1].constant", 1.0f);
			textureShader.setFloat("pointLights[1].linear", 0.22f);
			textureShader.setFloat("pointLights[1].quadratic", 0.20f);
			textureShader.setVec3("pointLights[1].ambient", lightColor * 0.05f);
			textureShader.setVec3("pointLights[1].diffuse", lightColor * 0.4f);
			textureShader.setVec3("pointLights[1].specular", glm::vec3(0.5f));

			lightModel = glm::mat4(1.0f);
			lightModel = glm::rotate(lightModel, glm::radians(angleVal), glm::vec3(0.0f, 1.0f, 0.0f));
			lightModel = glm::translate(lightModel, pointLightPositions[0]);
			lightModel = glm::scale(lightModel, glm::vec3(0.2f));
			newLightPos = (lightModel * glm::vec4(1.0f));

			textureShader.setVec3("pointLights[2].position", newLightPos);
			textureShader.setFloat("pointLights[2].constant", 1.0f);
			textureShader.setFloat("pointLights[2].linear", 0.22f);
			textureShader.setFloat("pointLights[2].quadratic", 0.20f);
			textureShader.setVec3("pointLights[2].ambient", lightColor * 0.05f);
			textureShader.setVec3("pointLights[2].diffuse", lightColor * 0.4f);
			textureShader.setVec3("pointLights[2].specular", glm::vec3(0.5f));

			lightModel = glm::mat4(1.0f);
			lightModel = glm::rotate(lightModel, glm::radians(angleVal), glm::vec3(0.0f, 1.0f, 0.0f));
			lightModel = glm::translate(lightModel, pointLightPositions[0]);
			lightModel = glm::scale(lightModel, glm::vec3(0.2f));
			newLightPos = (lightModel * glm::vec4(1.0f));

			textureShader.setVec3("pointLights[3].position", newLightPos);
			textureShader.setFloat("pointLights[3].constant", 1.0f);
			textureShader.setFloat("pointLights[3].linear", 0.22f);
			textureShader.setFloat("pointLights[3].quadratic", 0.20f);
			textureShader.setVec3("pointLights[3].ambient", lightColor * 0.05f);
			textureShader.setVec3("pointLights[3].diffuse", lightColor * 0.4f);
			textureShader.setVec3("pointLights[3].specular", glm::vec3(0.5f));

			textureShader.setVec3("spotLight.position", camera.Position);
			textureShader.setVec3("spotLight.direction", camera.Front);
			textureShader.setFloat("spotLight.cutoff", glm::cos(glm::radians(12.5f)));
			textureShader.setFloat("spotLight.outerCutoff", glm::cos(glm::radians(20.0f)));
			textureShader.setVec3("spotLight.ambient", lightColor * 0.05f);
			textureShader.setVec3("spotLight.diffuse", lightColor * 0.3f);
			textureShader.setVec3("spotLight.specular", glm::vec3(0.5f));
			*/
		}
		/*
		// Bind Data
		glBindVertexArray(VAO); // bind vertex array to VAO
		// glDrawArrays(GL_TRIANGLES, 0, 3); // draw using vertices
		*/
		/*
		for (unsigned int i = 0; i < 10; i++)
		{
			// model matrix :: LOCAL TO WORLD
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 10.0f * (i + 1);
			if ((i + 1) % 2 == 0)
			{
				// model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			}
			else
			{
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			}
			mainShader.setMat4("model", model);
			lightingShader.setMat4("model", model);
			// gouraudShader.setMat4("model", model);
			textureShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		*/

		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // draw using vertices and indices
		// glBindVertexArray(0); //unbind vertex array

		// call events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	Log("Loop finished");
	/*
	// delete memory
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	*/
	// terminate program
	glfwTerminate();
	return 0;
}

// check exit input
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(FORWARD, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(LEFT, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(RIGHT, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(UP, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(DOWN, deltaTime);
		}
	}
}

// checks for background color
void processColor(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS)
	{
		glClearColor(0.5f, 0.1f, 0.1f, 1.0f); // Red
	}
	else if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
	{
		glClearColor(0.1f, 0.5f, 0.1f, 1.0f); // Green
	}
	else if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS)
	{
		glClearColor(0.1f, 0.1f, 0.5f, 1.0f); // Blue
	}
	else
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Grey
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// checks for draw mode
void processDraw(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe
	}
	else if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); // point
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // fill
	}
}

// Process slider value for which texture tor use
float processSlider(GLFWwindow *window, float sliderValue)
{
	int change = 0;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		change = +1;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		change = -1;
	}
	sliderValue += change * deltaTime;
	sliderValue = glm::clamp(sliderValue, 0.0f, 1.0f);
	return sliderValue;
}

// callback for mouse movement
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	if (isFirstMouse)
	{
		isFirstMouse = false;
		lastX = xpos;
		lastY = ypos;
	}
	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos; // y is reversed
	lastX = xpos;
	lastY = ypos;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		camera.ProcessMouseMovement(xOffset, yOffset);
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

// callback for the mouse scroll wheel
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

// callback on  window size change
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}