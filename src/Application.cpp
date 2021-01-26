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

	Primitive triangle(FileSystem::getPath("resources/primitives/2D/triangle.2d").c_str());
	Primitive lightObject(FileSystem::getPath("resources/primitives/3D/cube.3d").c_str());
	Primitive testCube(FileSystem::getPath("resources/primitives/3D/cube.3d").c_str());

	Shader shader2D(FileSystem::getPath("shaders/primitive/shader_2d.vs").c_str(), FileSystem::getPath("shaders/primitive/shader_2d.fs").c_str());
	// Light Source Shader
	Shader sourceShader(FileSystem::getPath("shaders/primitive/shader_source.vs").c_str(), FileSystem::getPath("shaders/primitive/shader_source.fs").c_str());
	// Shader colorShader(FileSystem::getPath("shaders/shader_source.vs").c_str(), FileSystem::getPath("shaders/shader_source.fs").c_str());
	// Shader textureShader(FileSystem::getPath("shaders/shader_source.vs").c_str(), FileSystem::getPath("shaders/shader_source.fs").c_str());
	// Shader materialShader(FileSystem::getPath("shaders/shader_source.vs").c_str(), FileSystem::getPath("shaders/shader_source.fs").c_str());
	// Shader modelShader(FileSystem::getPath("shaders/shader_source.vs").c_str(), FileSystem::getPath("shaders/shader_source.fs").c_str());

	vector<pTexture> textures2D;
	pTexture mainTex;
	mainTex.id = LoadTexture(FileSystem::getPath("resources/textures/awesomeface.png").c_str());
	mainTex.type = "texture_diffuse";
	textures2D.push_back(mainTex);
	// triangle.SetupTextures(textures2D);

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

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f, 0.2f, 2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f, 2.0f, -12.0f),
		glm::vec3(0.0f, 0.0f, -3.0f)};

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

	// Values setup
	float sliderValue = 0.5f;
	float timePeriod = 3.0f;
	float timeGap = 0.0f;
	float angleVal = 0.0f;

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

		// view matrix :: WORLD TO VIEW
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// projection matrix:: VIEW TO CLIPPED
		glm::mat4 projection;
		/*
		FOV = 45 degrees, RATIO = HEIGHT/WIDTH, Near Plane = 0.1, Far Plane = 100.0
		*/
		projection = glm::perspective(glm::radians(camera.Zoom), ((float)SCR_WIDTH) / ((float)SCR_HEIGHT), 0.1f, 100.0f);

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
		for (int i = 0; i < 4; i++)
		{
			glm::mat4 model = lightModel;
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			sourceShader.setMat4("model", model);
			lightObject.Draw(sourceShader);
		}

		for (unsigned int i = 0; i < 10; i++)
		{
			// model matrix :: LOCAL TO WORLD
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 15.0f * (i);
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			sourceShader.setMat4("model", model);
			testCube.Draw(sourceShader);
		}
		// shader2D.use();
		// triangle.Draw(shader2D);

		// call events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	Log("Loop finished");

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