// Library headers
#include <glad/glad.h>	// first glad
#include <GLFW/glfw3.h> // then glfw
#include <stb_image.h>	// for loading images

// glm libraries for math
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

// imgui headers
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

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
void setupGLFW(int major, int minor);
void setupData(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
bool checkInput(GLFWwindow *window, int key);
void processInput(GLFWwindow *window);
void processColor(GLFWwindow *window);
void setColor(float r, float g, float b, float a);
void processDraw(bool *isLine, bool *isPoint, bool *isFill);
void setDraw(int choice);
float processSlider(GLFWwindow *window, float sliderValue);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void setScene(Shader &shader, glm::vec3 lightColor, float angleVal, glm::vec3 pointLightPositions[], int numberOfLights);

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

// Input Values
bool canMoveCamera = false;
bool canRotateCamera = false;

// main function
int main()
{
	// OpenGL version 3.3
	setupGLFW(3, 3);
	// Window intialize
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Window", NULL, NULL);
	if (window == NULL)
	{
		Log("Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}
	setupData(window);
	// check GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Log("Failed to intialize GLAD");
		return -1;
	}

	glEnable(GL_DEPTH_TEST);				// Enable Z buffering
	stbi_set_flip_vertically_on_load(true); // set before loading model

	// Setup ImGui
	const char *glsl_version = "#version 330";
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();

	Primitive triangle(FileSystem::getPath("resources/primitives/2D/triangle.2d").c_str());
	Primitive lightObject(FileSystem::getPath("resources/primitives/3D/cube.3d").c_str());
	Primitive testCube(FileSystem::getPath("resources/primitives/3D/cube.3d").c_str());

	// Model mainModel(FileSystem::getPath("resources/models/backpack/backpack.obj"));
	Model mainModel(FileSystem::getPath("resources/models/sphere/sphere.obj"));

	Shader shader2D(FileSystem::getPath("shaders/primitive/shader_2d.vs").c_str(), FileSystem::getPath("shaders/primitive/shader_2d.fs").c_str());
	// Light Source Shader
	Shader sourceShader(FileSystem::getPath("shaders/primitive/shader_source.vs").c_str(), FileSystem::getPath("shaders/primitive/shader_source.fs").c_str());
	Shader shader3D(FileSystem::getPath("shaders/primitive/shader_3d.vs").c_str(), FileSystem::getPath("shaders/primitive/shader_3d_scene.fs").c_str());
	Shader modelShader(FileSystem::getPath("shaders/light/shader_model.vs").c_str(), FileSystem::getPath("shaders/light/shader_model.fs").c_str());

	vector<pTexture> textures2D;
	pTexture mainTex;
	mainTex.id = LoadTexture(FileSystem::getPath("resources/textures/awesomeface.png").c_str());
	mainTex.type = "texture_diffuse";
	textures2D.push_back(mainTex);
	triangle.SetupTextures(textures2D);

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
	testCube.SetupTextures(textures3D);

	glm::vec3 lightColor(1.0f);
	glm::vec3 objectColor(1.0f, 0.5f, 0.31f);

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

	// UI Data
	bool showTraingle = false;
	bool showCubes = false;
	ImVec4 backgroundColor = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);

	bool renderLines = false;
	bool renderPoint = false;
	bool renderFill = true;

	bool pRenderLine;
	bool pRenderPoint;
	bool pRenderFill;

	bool showFrameRate = false;

	float posX, posY, posZ, rotX, rotY, rotZ, sclX, sclY, sclZ;
	posX = 0;
	posY = 0;
	posZ = 0;
	rotX = 0;
	rotY = 0;
	rotZ = 0;
	sclX = 1;
	sclY = 1;
	sclZ = 1;
	bool globalRotation = false;

	while (!glfwWindowShouldClose(window))
	{
		// Setup bools
		pRenderLine = renderLines;
		pRenderPoint = renderPoint;
		pRenderFill = renderFill;

		// Setup Imgui Frame data
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

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
		// processColor(window);
		setColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w);
		// processDraw(window);
		processDraw(&renderLines, &renderPoint, &renderFill);

		// view matrix :: WORLD TO VIEW
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// projection matrix:: VIEW TO CLIPPED
		glm::mat4 projection;
		/*
		FOV = X degrees, RATIO =WIDTH/HEIGHT, Near Plane = 0.1, Far Plane = 100.0
		*/
		projection = glm::perspective(glm::radians(camera.Zoom), ((float)SCR_WIDTH) / ((float)SCR_HEIGHT), 0.1f, 100.0f);

		glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
		glm::mat4 lightModel = glm::mat4(1.0f);
		if (checkInput(window, GLFW_KEY_KP_ADD))
		{
			angleVal += deltaTime * (360.0f / timePeriod);
			if (angleVal > 360)
			{
				angleVal -= 360;
			}
		}
		if (checkInput(window, GLFW_KEY_KP_SUBTRACT))
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
		shader3D.use();
		shader3D.setMat4("view", view);
		shader3D.setMat4("projection", projection);

		setScene(shader3D, lightColor, angleVal, pointLightPositions, 4);
		shader3D.setVec3("viewPos", camera.Position);
		shader3D.setFloat("material.shininess", 64);
		shader3D.setVec3("material.ambient", objectColor);
		shader3D.setVec3("material.diffuse", objectColor);
		shader3D.setVec3("material.specular", 0.5f, 0.5f, 0.5f);

		for (unsigned int i = 0; i < 10; i++)
		{
			// model matrix :: LOCAL TO WORLD
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 15.0f * (i);
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader3D.setMat4("model", model);
			if (showCubes)
			{
				testCube.Draw(shader3D);
			}
		}

		if (showTraingle)
		{
			shader2D.use();
			triangle.Draw(shader2D);
		}

		// Setup matrices
		glm::vec3 objectPos(posX, posY, posZ);
		glm::vec3 objectRot(rotX, rotY, rotZ);
		glm::vec3 objectScale(sclX, sclY, sclZ);
		glm::mat4 model(1.0f);
		model = glm::translate(model, objectPos);
		if (!globalRotation)
		{
			model = glm::rotate(model, glm::radians(objectRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(objectRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(objectRot.z), glm::vec3(0.0f, 0.0f, 1.0f));
		}
		else
		{
			glm::mat4 modelY = glm::rotate(model, glm::radians(objectRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 modelX = glm::rotate(model, glm::radians(objectRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
			glm::mat4 modelZ = glm::rotate(model, glm::radians(objectRot.z), glm::vec3(0.0f, 0.0f, 1.0f));
			model = modelX * modelY * modelZ * model;
		}
		model = glm::scale(model, objectScale);

		modelShader.use();
		modelShader.setMat4("view", view);
		modelShader.setMat4("projection", projection);
		modelShader.setVec3("viewPos", camera.Position);
		modelShader.setFloat("material.shininess", 64);
		modelShader.setMat4("model", model);
		setScene(modelShader, lightColor, angleVal, pointLightPositions, 4);
		mainModel.Draw(modelShader);

		// Render your GUI
		ImGui::Begin("Debug Window");
		ImGui::Text("Setup OpenGL Render Data:-");
		ImGui::Checkbox("Show Cubes", &showCubes);
		ImGui::Checkbox("Show Triangle", &showTraingle);
		ImGui::ColorEdit3("Background Color", (float *)&backgroundColor);
		ImGui::Text("Fill Mode:- ");
		ImGui::SameLine();
		ImGui::Checkbox("Fill", &renderFill);
		ImGui::SameLine();
		ImGui::Checkbox("Line", &renderLines);
		ImGui::SameLine();
		ImGui::Checkbox("Point", &renderPoint);
		ImGui::Checkbox("Show FrameRate", &showFrameRate);
		if (showFrameRate)
		{
			ImGui::SameLine();
			int val = (int)(1.0f / deltaTime);
			ImGui::Text("FrameRate:- %d", val);
		}
		ImGui::SliderFloat("Light Rotation", &angleVal, 0.0f, 360.0f);
		ImGui::Checkbox("Move Camera", &canMoveCamera);
		ImGui::Checkbox("Rotate Camera", &canRotateCamera);
		ImGui::Checkbox("GlobalRotation", &globalRotation);
		ImGui::Text("Set Transform:- ");
		ImGui::Text("Position: ");
		ImGui::SliderFloat("X##1", &posX, -5.0f, 5.0f);
		ImGui::SliderFloat("Y##1", &posY, -5.0f, 5.0f);
		ImGui::SliderFloat("Z##1", &posZ, -5.0f, 5.0f);
		ImGui::Text("Rotation: ");
		ImGui::SliderFloat("X##2", &rotX, -360.0f, 360.0f);
		ImGui::SliderFloat("Y##2", &rotY, -360.0f, 360.0f);
		ImGui::SliderFloat("Z##2", &rotZ, -360.0f, 360.0f);
		ImGui::Text("Scale: ");
		ImGui::SliderFloat("X##3", &sclX, 0.0f, 5.0f);
		ImGui::SliderFloat("Y##3", &sclY, 0.0f, 5.0f);
		ImGui::SliderFloat("Z##3", &sclZ, 0.0f, 5.0f);
		if (ImGui::Button("Reset Object"))
		{
			posX = 0;
			posY = 0;
			posZ = 0;
			rotX = 0;
			rotY = 0;
			rotZ = 0;
			sclX = 1;
			sclY = 1;
			sclZ = 1;
		}
		ImGui::End();

		// Check bools
		if (renderFill != pRenderFill && renderFill)
		{
			renderLines = false;
			renderPoint = false;
		}
		else if (renderPoint != pRenderPoint && renderPoint)
		{
			renderLines = false;
			renderFill = false;
		}
		else if (renderLines != pRenderLine && renderLines)
		{
			renderFill = false;
			renderPoint = false;
		}
		if (!renderFill && !renderLines && !renderPoint)
		{
			renderFill = true;
		}

		// Render calls imgui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// call events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// Terminate Imgui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// terminate program
	glfwTerminate();
	return 0;
}

void setupGLFW(int major, int minor)
{
	// glfw initialise
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);			   // open gl version 3.x
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);			   // version is 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // use core profile
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);		   // for Mac
}

void setupData(GLFWwindow *window)
{
	glfwMakeContextCurrent(window);									   // current window
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // callback set for size change
	glfwSetCursorPosCallback(window, mouse_callback);				   // callback for mouse movement
	glfwSetScrollCallback(window, scroll_callback);					   // callback for zoom
																	   //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);// disable cursor
}

bool checkInput(GLFWwindow *window, int key)
{
	return (glfwGetKey(window, key) == GLFW_PRESS);
}

// check exit input
void processInput(GLFWwindow *window)
{
	if (checkInput(window, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (canMoveCamera)
	{
		if (checkInput(window, GLFW_KEY_W))
		{
			camera.ProcessKeyboard(FORWARD, deltaTime);
		}
		if (checkInput(window, GLFW_KEY_S))
		{
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		}
		if (checkInput(window, GLFW_KEY_A))
		{
			camera.ProcessKeyboard(LEFT, deltaTime);
		}
		if (checkInput(window, GLFW_KEY_D))
		{
			camera.ProcessKeyboard(RIGHT, deltaTime);
		}
		if (checkInput(window, GLFW_KEY_UP))
		{
			camera.ProcessKeyboard(UP, deltaTime);
		}
		if (checkInput(window, GLFW_KEY_DOWN))
		{
			camera.ProcessKeyboard(DOWN, deltaTime);
		}
	}
}

// checks for background color
void processColor(GLFWwindow *window)
{
	if (checkInput(window, GLFW_KEY_KP_1))
	{
		setColor(0.5f, 0.1f, 0.1f, 1.0f); // Red
	}
	else if (checkInput(window, GLFW_KEY_KP_2))
	{
		setColor(0.1f, 0.5f, 0.1f, 1.0f); // Green
	}
	else if (checkInput(window, GLFW_KEY_KP_3))
	{
		setColor(0.1f, 0.1f, 0.5f, 1.0f); // Blue
	}
	else
	{
		setColor(0.1f, 0.1f, 0.1f, 1.0f); // Grey
	}
}

// Sets color of background
void setColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a); // Grey
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// checks for draw mode
void processDraw(bool *isLine, bool *isPoint, bool *isFill)
{
	int choice = 2;
	if (*isLine)
	{
		choice = 0;
	}
	else if (*isPoint)
	{
		choice = 1;
	}
	setDraw(choice);
}

// Sets Draw Mode
void setDraw(int choice)
{
	GLenum fillMode = GL_FILL;
	if (choice == 0)
	{
		fillMode = GL_LINE; // wireframe
	}
	else if (choice == 1)
	{
		fillMode = GL_POINT; // wireframe
	}
	glPolygonMode(GL_FRONT_AND_BACK, fillMode);
}

// Process slider value for which texture tor use
float processSlider(GLFWwindow *window, float sliderValue)
{
	int change = 0;
	if (checkInput(window, GLFW_KEY_LEFT))
	{
		change = +1;
	}
	else if (checkInput(window, GLFW_KEY_RIGHT))
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
	if (canRotateCamera)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		camera.ProcessMouseMovement(xOffset, yOffset, deltaTime);
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

// Sets the Scene Light Properties
void setScene(Shader &shader, glm::vec3 lightColor, float angleVal, glm::vec3 pointLightPositions[], int numberOfLights)
{
	shader.setVec3("dirLight.direction", glm::vec3(0.5f, -1.0f, -0.7f));
	shader.setVec3("dirLight.ambient", lightColor * 0.1f);
	shader.setVec3("dirLight.diffuse", lightColor * 0.25f);
	shader.setVec3("dirLight.specular", glm::vec3(0.5f));

	for (int i = 0; i < numberOfLights; i++)
	{
		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::rotate(lightModel, glm::radians(angleVal), glm::vec3(0.0f, 1.0f, 0.0f));
		lightModel = glm::translate(lightModel, pointLightPositions[i]);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));
		glm::vec3 newLightPos = (lightModel * glm::vec4(1.0f));

		std::string lightName = "pointLights[" + std::to_string(i) + "].";
		shader.setVec3(lightName + "position", newLightPos);
		shader.setFloat(lightName + "constant", 1.0f);
		shader.setFloat(lightName + "linear", 0.22f);
		shader.setFloat(lightName + "quadratic", 0.20f);
		shader.setVec3(lightName + "ambient", lightColor * 0.05f);
		shader.setVec3(lightName + "diffuse", lightColor * 0.4f);
		shader.setVec3(lightName + "specular", glm::vec3(1.0f));
	}

	shader.setVec3("spotLight.position", camera.Position);
	shader.setVec3("spotLight.direction", camera.Front);
	shader.setFloat("spotLight.cutoff", glm::cos(glm::radians(12.5f)));
	shader.setFloat("spotLight.outerCutoff", glm::cos(glm::radians(20.0f)));
	shader.setVec3("spotLight.ambient", lightColor * 0.05f);
	shader.setVec3("spotLight.diffuse", lightColor * 0.3f);
	shader.setVec3("spotLight.specular", glm::vec3(0.5f));
}