//libraries
#include <glad/glad.h>	//first glad
#include <GLFW/glfw3.h> //then glfw
#include <stb_image.h>	//for loading images

//glm libraries for math
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <Shader.h> //shader header
#include <Camera.h> //camera header
#include <iostream>

//functions
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void processColor(GLFWwindow *window);
void processDraw(GLFWwindow *window);
float processSlider(GLFWwindow *window, float sliderValue);
unsigned int generateTexture(const std::string &name, bool isPNG);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

//const settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//camera settings
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool isFirstMouse = true;
//frame settings
float lastFrameTime = 0.0f;
float deltaTime = 0.0f;

//main function
int main()
{
	//glfw initialise
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);				   //open gl version 3.x
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);				   //version is 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //use core profile
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //for Mac

	//window intialize
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Window", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);									   //current window
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //callback set for size change
	glfwSetCursorPosCallback(window, mouse_callback);				   //callback for mouse movement
	glfwSetScrollCallback(window, scroll_callback);					   //callback for zoom

	//check GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to intialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST); //Enable Z buffering
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//disable cursor

	//Vertex Data
	//triangle
	/*float vertices[] = {
		//vertices         //colors
		-0.5f,-0.5f,0.0f, 1.0f,0.0f,0.0f,
		0.0f,0.5f,0.0f,   0.0f,1.0f,0.0f,
		0.5f,-0.5f,0.0f,  0.0f,0.0f,1.0f
	};
	unsigned int indices[] = {
		0,1,2
	};*/

	//rectangle
	/*float vertices[] = {
		//vertices        //colors         //texture coord
		0.5f,0.5f,0.0f,   1.0f,0.0f,0.0f,  1.0f,1.0f,
		0.5f,-0.5f,0.0f,  0.0f,1.0f,0.0f,  1.0f,0.0f,
		-0.5f,-0.5f,0.0f, 0.0f,0.0f,1.0f,  0.0f,0.0f,
		-0.5f,0.5f,0.0f,  1.0f,1.0f,1.0f,  0.0f,1.0f
	};
	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};*/

	//cube vertices with texture
	float vertices[] = {
		//vertices            //color         //texture UV
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
	//world POS for each instance of cube
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

	//GPU Memory Setup
	//initialise variables
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO); //VAO stores vertex attributes
	glGenBuffers(1, &VBO);		//VBO stores vertex data in bulk
	glGenBuffers(1, &EBO);		//EBO stores indices of vertices
	//Bind Local variables
	//Link VAO
	glBindVertexArray(VAO); //Link Vertex Array to VAO
	//Link VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);										   //Link Current Array Buffer to VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //Map vertices to Array Buffer i.e. VBO
	/*//Link EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);//Link Current Element Array Buffer to EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);//Map Vertex Indices to Element Array Buffer i.e. EBO*/
	//Setup Attributes
	//layout location,elements in each attribute,data type,to normalize,stride gap,start index
	/*glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//format of vertices to be read
	glEnableVertexAttribArray(0);//enables Array*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//Clear Memory
	//first VBO then VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind array buffer
	glBindVertexArray(0);			  //unbind vertex Array

	//Create shader
	Shader mainShader("../shaders/shader.vs", "../shaders/shader.fs");

	//Texture setup
	unsigned int texture, texture2;
	texture = generateTexture("../resources/textures/container.jpg", false);
	texture2 = generateTexture("../resources/textures/awesomeface.png", true);
	mainShader.use();
	mainShader.setInt("texture1", 0);
	mainShader.setInt("texture2", 1);

	//values setup
	float sliderValue = 0.5f;
	float timePeriod = 3.0f;
	int gapLoc = glGetUniformLocation(mainShader.ID, "timeGap");
	int sliderLoc = glGetUniformLocation(mainShader.ID, "slider");
	mainShader.setFloat("timePeriod", timePeriod);
	float timeGap = 0.0f;

	//render loop
	while (!glfwWindowShouldClose(window))
	{
		//delta time refresh
		float currentFrameTime = glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;
		timeGap += deltaTime;
		if (timeGap >= timePeriod)
		{
			timeGap -= timePeriod;
		}

		//input commands
		processInput(window);
		processColor(window);
		processDraw(window);

		//bind textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		//use Shader
		mainShader.use();
		sliderValue = processSlider(window, sliderValue);
		glUniform1f(sliderLoc, sliderValue);
		glUniform1f(gapLoc, timeGap);

		//view matrix :: WORLD TO VIEW
		glm::mat4 view;
		view = camera.GetViewMatrix();
		mainShader.setMat4("view", view);

		//projection matrix:: VIEW TO CLIPPED
		glm::mat4 projection;
		//FOV=45 degrees,RATIO=HEIGHT/WIDTH,Near Plane=0.1,Far Plane=100.0
		projection = glm::perspective(glm::radians(camera.Zoom), ((float)SCR_WIDTH) / ((float)SCR_HEIGHT), 0.1f, 100.0f);
		mainShader.setMat4("projection", projection);

		//Bind Data
		glBindVertexArray(VAO); //bind vertex array to VAO
		//glDrawArrays(GL_TRIANGLES, 0, 3); //draw using vertices

		for (unsigned int i = 0; i < 10; i++)
		{
			//model matrix :: LOCAL TO WORLD
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * (i + 1);
			if (i % 2 == 0)
			{
				model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			}
			else
			{
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			}
			mainShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //draw using vertices and indices
		//glBindVertexArray(0); //unbind vertex array

		//call events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//delete memory
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	//terminate program
	glfwTerminate();
	return 0;
}

//check exit input
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

//checks for background color
void processColor(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS)
	{
		glClearColor(0.5f, 0.1f, 0.1f, 1.0f); //Red
	}
	else if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
	{
		glClearColor(0.1f, 0.5f, 0.1f, 1.0f); //Green
	}
	else if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS)
	{
		glClearColor(0.1f, 0.1f, 0.5f, 1.0f); //Blue
	}
	else
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //Grey
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//checks for draw mode
void processDraw(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe
	}
	else if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); //point
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //fill
	}
}

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
	if (sliderValue > 1.0f)
	{
		sliderValue = 1.0f;
	}
	else if (sliderValue < 0.0f)
	{
		sliderValue = 0.0f;
	}
	return sliderValue;
}

unsigned int generateTexture(const std::string &name, bool isPNG)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//setup texture wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//setup data
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(name.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		if (isPNG)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return texture;
}

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

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{

	camera.ProcessMouseScroll(yoffset);
}

//callback on size changee
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}