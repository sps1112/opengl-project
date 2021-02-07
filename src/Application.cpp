// glm libraries for math
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtc/quaternion.hpp>
#include <glm/glm/gtx/quaternion.hpp>

// imgui headers
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

// Custom Headers
#include <Renderer.h>	// Renderer header
#include <Shader.h>		// shader header
#include <Camera.h>		// camera header
#include <Model.h>		// model header
#include <FileSystem.h> // Filesystem header
#include <Utils.h>		// Utility header
#include <Primitive.h>	// Primitive header

#include <iostream>
#include <vector>

// Const settings
const int majorVersion = 3;
const int minorVersion = 3;
const char *windowTitle = "OpenGL Window";
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Frame settings
float lastFrameTime = 0.0f;
float deltaTime = 0.0f;

// Input Values
bool canMoveCamera = false;
bool canRotateCamera = false;

// main function
int main()
{
	Renderer renderer(majorVersion, minorVersion, SCR_WIDTH, SCR_HEIGHT);
	renderer.SetupGLFW();
	renderer.CreateWindow(windowTitle);
	if (renderer.window == NULL)
	{
		renderer.TerminateGLFW();
		return -1;
	}
	renderer.SetData();
	// check GLAD
	if (!renderer.checkGLAD())
	{
		renderer.TerminateGLFW();
		return -1;
	}
	renderer.SetOtherData();

	// Camera settings
	glm::vec3 cameraPos(0.0f, 0.0f, 5.0f);
	Camera localCam(cameraPos);
	renderer.SetCamera(localCam);

	// Setup ImGui
	std::string versionText = "#version " + std::to_string(majorVersion) + std::to_string(minorVersion) + "0";
	const char *glsl_version = versionText.c_str();
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(renderer.window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();

	Primitive triangle(FileSystem::getPath("resources/primitives/2D/triangle.2d").c_str());
	Primitive lightObject(FileSystem::getPath("resources/primitives/3D/cube.3d").c_str());
	Primitive testCube(FileSystem::getPath("resources/primitives/3D/cube.3d").c_str());

	Model mainModel(FileSystem::getPath("resources/models/backpack/backpack.obj"));
	// Model mainModel(FileSystem::getPath("resources/models/teapot/teapot.obj"));

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
	ImVec4 lightColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

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
	while (!glfwWindowShouldClose(renderer.window))
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
		renderer.ProcessInput(canMoveCamera, deltaTime);
		renderer.SetColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w);
		renderer.ProcessDraw(renderLines, renderPoint, renderFill);
		renderer.ProcessMouse(canRotateCamera, deltaTime);

		// view matrix :: WORLD TO VIEW
		glm::mat4 view;
		view = (*(renderer.GetCamera())).GetViewMatrix();

		// projection matrix:: VIEW TO CLIPPED
		glm::mat4 projection;
		/*
		FOV = X degrees, RATIO =WIDTH/HEIGHT, Near Plane = 0.1, Far Plane = 100.0
		*/
		projection = glm::perspective(glm::radians(renderer.GetZoom()), ((float)SCR_WIDTH) / ((float)SCR_HEIGHT), 0.1f, 100.0f);

		glm::vec3 currentLightColor(lightColor.x, lightColor.y, lightColor.z);
		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::rotate(lightModel, glm::radians(angleVal), glm::vec3(0.0f, 1.0f, 0.0f));
		sourceShader.use();
		sourceShader.setMat4("view", view);
		sourceShader.setMat4("projection", projection);
		sourceShader.setVec3("sourceColor", currentLightColor);
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

		shader3D.SetScene(currentLightColor, angleVal, pointLightPositions, 4, (*(renderer.GetCamera())).Position, (*(renderer.GetCamera())).Front);
		shader3D.setVec3("viewPos", (*(renderer.GetCamera())).Position);
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
		/*glm::quat rotQuat(glm::radians(objectRot));
		model = glm::mat4(rotQuat) * model;*/
		if (!globalRotation)
		{
			model = glm::rotate(model, glm::radians(objectRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(objectRot.z), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, glm::radians(objectRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else
		{
			glm::mat4 modelY = glm::rotate(model, glm::radians(objectRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 modelZ = glm::rotate(model, glm::radians(objectRot.z), glm::vec3(0.0f, 0.0f, 1.0f));
			glm::mat4 modelX = glm::rotate(model, glm::radians(objectRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = (modelX * (modelZ * (modelY * model)));
		}
		/*glm::mat4 rotationMat(1.0f);
		rotationMat = glm::rotate(rotationMat, glm::radians(objectRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
		rotationMat = glm::rotate(rotationMat, glm::radians(objectRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
		rotationMat = glm::rotate(rotationMat, glm::radians(objectRot.z), glm::vec3(0.0f, 0.0f, 1.0f));
		if (!globalRotation)
		{
			model = model * rotationMat;
		}
		else
		{
			model = rotationMat * model;
		}*/
		model = glm::scale(model, objectScale);

		modelShader.use();
		modelShader.setMat4("view", view);
		modelShader.setMat4("projection", projection);
		modelShader.setVec3("viewPos", (*(renderer.GetCamera())).Position);
		modelShader.setFloat("material.shininess", 64);
		modelShader.setMat4("model", model);
		modelShader.SetScene(currentLightColor, angleVal, pointLightPositions, 4, (*(renderer.GetCamera())).Position, (*(renderer.GetCamera())).Front);
		mainModel.Draw(modelShader);

		// Render your GUI
		ImGui::Begin("Debug Window");
		ImGui::Text("Setup OpenGL Render Data:-");
		ImGui::Checkbox("Show Cubes", &showCubes);
		ImGui::Checkbox("Show Triangle", &showTraingle);
		ImGui::ColorEdit3("Light Color", (float *)&lightColor);
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
		glfwSwapBuffers(renderer.window);
		glfwPollEvents();
	}
	// Terminate Imgui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// terminate program
	renderer.TerminateGLFW();
	return 0;
}