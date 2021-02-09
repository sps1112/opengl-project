// Custom Headers
#include <Renderer.h>	// Renderer header
#include <Math.h>		// Math header
#include <Shader.h>		// Shader header
#include <Camera.h>		// Camera header
#include <Texture.h>	// Texture header
#include <Model.h>		// Model header
#include <FileSystem.h> // Filesystem header
#include <Utils.h>		// Utility header
#include <Primitive.h>	// Primitive header
#include <GUI.h>		// GUI header

#include <iostream>
#include <vector>

// Const settings
const int majorVersion = 3;
const int minorVersion = 3;
const char *windowTitle = "OpenGL Window";
std::string guiTitle = "UI Window";
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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
	GUI gui(renderer.window, majorVersion, minorVersion);
	GUIWindow guiWindow(guiTitle);

	Primitive triangle(FileSystem::getPath("resources/primitives/2D/triangle.2d").c_str());
	Primitive lightObject(FileSystem::getPath("resources/primitives/3D/cube.3d").c_str());
	Primitive testCube(FileSystem::getPath("resources/primitives/3D/cube.3d").c_str());

	Model mainModel(FileSystem::getPath("resources/models/backpack/backpack.obj"));

	Shader shader2D(FileSystem::getPath("shaders/primitive/shader_2d.vs").c_str(), FileSystem::getPath("shaders/primitive/shader_2d.fs").c_str());
	Shader sourceShader(FileSystem::getPath("shaders/primitive/shader_source.vs").c_str(), FileSystem::getPath("shaders/primitive/shader_source.fs").c_str());
	Shader shader3DMat(FileSystem::getPath("shaders/primitive/shader_3d.vs").c_str(), FileSystem::getPath("shaders/primitive/shader_3d_mat.fs").c_str());
	Shader shader3D(FileSystem::getPath("shaders/primitive/shader_3d.vs").c_str(), FileSystem::getPath("shaders/primitive/shader_3d_scene.fs").c_str());
	Shader modelShader(FileSystem::getPath("shaders/light/shader_model.vs").c_str(), FileSystem::getPath("shaders/light/shader_model.fs").c_str());

	vector<Texture> textures2D;
	Texture mainTex;
	mainTex.id = LoadTextureFromPath(FileSystem::getPath("resources/textures/awesomeface.png").c_str());
	mainTex.type = "texture_diffuse";
	textures2D.push_back(mainTex);
	triangle.SetupTextures(textures2D);

	vector<Texture> textures3D;
	Texture diffuse, specular, emmision;
	diffuse.id = LoadTextureFromPath(FileSystem::getPath("resources/textures/container2.png").c_str());
	diffuse.type = "texture_diffuse";
	textures3D.push_back(diffuse);
	specular.id = LoadTextureFromPath(FileSystem::getPath("resources/textures/container2_specular.png").c_str());
	specular.type = "texture_specular";
	textures3D.push_back(specular);
	emmision.id = LoadTextureFromPath(FileSystem::getPath("resources/textures/matrix.jpg").c_str());
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
	float angleVal = 0.0f;

	// UI Data
	bool showTriangles = false;
	bool showCubes = false;
	bool colorCubes = false;
	// Input Values
	bool canMoveCamera = false;
	bool canRotateCamera = false;

	ImVec4 backgroundColor = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
	ImVec4 lightColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 cubeColor = ImVec4(objectColor.x, objectColor.y, objectColor.z, 1.0f);

	bool renderLines = false;
	bool renderPoint = false;
	bool renderFill = true;

	bool pRenderLine;
	bool pRenderPoint;
	bool pRenderFill;

	bool showFrameRate = false;
	Transform objectTransform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	bool globalRotation = false;
	guiWindow.AddGUI(GUI_LINE, "Setup OpenGL Render Data:-", true);
	guiWindow.AddGUI(GUI_CHECKBOX, "Show Cubes", true, &showCubes);
	guiWindow.AddGUI(GUI_CHECKBOX, "Show Triangles", true, &showTriangles);
	guiWindow.AddGUI(GUI_COLOR, "Light Color", true, true, &lightColor);
	guiWindow.AddGUI(GUI_COLOR, "Background Color", true, true, &backgroundColor);
	guiWindow.AddGUI(GUI_CHECKBOX, "Color Cubes", true, &colorCubes);
	guiWindow.AddGUI(GUI_COLOR, "Cube Color", true, true, &cubeColor);
	guiWindow.AddGUI(GUI_LINE, "Fill Mode:- ", true);
	guiWindow.AddGUI(GUI_CHECKBOX, "Fill", false, &renderFill);
	guiWindow.AddGUI(GUI_CHECKBOX, "Line", false, &renderLines);
	guiWindow.AddGUI(GUI_CHECKBOX, "Point", false, &renderPoint);
	guiWindow.AddGUI(GUI_FLOAT, "Light Rotation", true, &angleVal, 0.0f, 360.0f);
	guiWindow.AddGUI(GUI_CHECKBOX, "Move Camera", true, &canMoveCamera);
	guiWindow.AddGUI(GUI_CHECKBOX, "Rotate Camera", true, &canRotateCamera);
	guiWindow.AddGUI(GUI_CHECKBOX, "Global Roatation", true, &globalRotation);
	guiWindow.AddGUI(GUI_LINE, "Set Transform:-", true);
	guiWindow.AddGUI(GUI_LINE, "Position:-", true);
	guiWindow.AddGUI(GUI_FLOAT, "X##1", true, &objectTransform.position.x, -5.0f, 5.0f);
	guiWindow.AddGUI(GUI_FLOAT, "Y##1", true, &objectTransform.position.y, -5.0f, 5.0f);
	guiWindow.AddGUI(GUI_FLOAT, "Z##1", true, &objectTransform.position.z, -5.0f, 5.0f);
	guiWindow.AddGUI(GUI_LINE, "Rotation:-", true);
	guiWindow.AddGUI(GUI_FLOAT, "X##2", true, &objectTransform.rotation.x, -360.0f, 360.0f);
	guiWindow.AddGUI(GUI_FLOAT, "Y##2", true, &objectTransform.rotation.y, -360.0f, 360.0f);
	guiWindow.AddGUI(GUI_FLOAT, "Z##2", true, &objectTransform.rotation.z, -360.0f, 360.0f);
	guiWindow.AddGUI(GUI_LINE, "Scale:-", true);
	guiWindow.AddGUI(GUI_FLOAT, "X##3", true, &objectTransform.scale.x, 0.001f, 5.0f);
	guiWindow.AddGUI(GUI_FLOAT, "Y##3", true, &objectTransform.scale.y, 0.001f, 5.0f);
	guiWindow.AddGUI(GUI_FLOAT, "Z##3", true, &objectTransform.scale.z, 0.001f, 5.0f);

	renderer.StartTimer();
	while (!glfwWindowShouldClose(renderer.window))
	{
		// Setup bools
		pRenderLine = renderLines;
		pRenderPoint = renderPoint;
		pRenderFill = renderFill;

		// Setup Imgui Frame data
		gui.NewFrame();

		// delta time refresh
		renderer.NewFrame();

		// input commands
		renderer.ProcessInput(canMoveCamera);
		renderer.SetColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w);
		renderer.ProcessDraw(renderLines, renderPoint, renderFill);
		renderer.ProcessMouse(canRotateCamera);

		// view matrix :: WORLD TO VIEW
		glm::mat4 view;
		view = (*(renderer.GetCamera())).GetViewMatrix();

		// projection matrix:: VIEW TO CLIPPED
		glm::mat4 projection;
		// FOV = X degrees, RATIO =WIDTH/HEIGHT, Near Plane = 0.1, Far Plane = 100.0
		projection = glm::perspective(glm::radians(renderer.GetZoom()), ((float)SCR_WIDTH) / ((float)SCR_HEIGHT), 0.1f, 100.0f);

		glm::vec3 currentLightColor(lightColor.x, lightColor.y, lightColor.z);
		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::rotate(lightModel, glm::radians(angleVal), glm::vec3(0.0f, 1.0f, 0.0f));
		sourceShader.use();
		sourceShader.setVec3("sourceColor", currentLightColor);
		for (int i = 0; i < 4; i++)
		{
			glm::mat4 model = lightModel;
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			sourceShader.SetMatrices(model, view, projection);
			lightObject.Draw(sourceShader);
		}

		if (colorCubes)
		{
			glm::vec3 currentCubeColor(cubeColor.x, cubeColor.y, cubeColor.z);
			shader3DMat.use();
			shader3DMat.SetScene(currentLightColor, angleVal, pointLightPositions, 4, (*(renderer.GetCamera())).Position, (*(renderer.GetCamera())).Front);
			shader3DMat.setVec3("viewPos", (*(renderer.GetCamera())).Position);
			shader3DMat.SetMaterial(currentCubeColor, currentCubeColor, currentLightColor * 0.5f, 64);

			for (unsigned int i = 0; i < 10; i++)
			{
				// model matrix :: LOCAL TO WORLD
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, cubePositions[i]);
				float angle = 15.0f * (i);
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				shader3DMat.SetMatrices(model, view, projection);
				if (showCubes)
				{
					testCube.Draw(shader3DMat);
				}
			}
		}
		else
		{
			shader3D.use();
			shader3D.SetScene(currentLightColor, angleVal, pointLightPositions, 4, (*(renderer.GetCamera())).Position, (*(renderer.GetCamera())).Front);
			shader3D.setVec3("viewPos", (*(renderer.GetCamera())).Position);
			shader3D.setFloat("material.shininess", 64);
			for (unsigned int i = 0; i < 10; i++)
			{
				// model matrix :: LOCAL TO WORLD
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, cubePositions[i]);
				float angle = 15.0f * (i);
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				shader3D.SetMatrices(model, view, projection);
				if (showCubes)
				{
					testCube.Draw(shader3D);
				}
			}
		}

		if (showTriangles)
		{
			shader2D.use();
			triangle.Draw(shader2D);
		}

		// Setup matrices
		glm::vec3 objectPos(objectTransform.position);
		glm::vec3 objectRot(objectTransform.rotation);
		glm::vec3 objectScale(objectTransform.scale);
		glm::mat4 model(1.0f);
		model = glm::translate(model, objectPos);
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
		model = glm::scale(model, objectScale);

		modelShader.use();
		modelShader.SetMatrices(model, view, projection);
		modelShader.setVec3("viewPos", (*(renderer.GetCamera())).Position);
		modelShader.setFloat("material.shininess", 64);
		modelShader.SetScene(currentLightColor, angleVal, pointLightPositions, 4, (*(renderer.GetCamera())).Position, (*(renderer.GetCamera())).Front);
		mainModel.Draw(modelShader);

		// Render your GUI
		guiWindow.ShowGUI();
		if (ImGui::Button("Reset Object"))
		{
			objectTransform.ResetToOrigin();
		}
		ImGui::Checkbox("Show FrameRate", &showFrameRate);
		if (showFrameRate)
		{
			ImGui::SameLine();
			int val = (int)(1.0f / renderer.deltaTime);
			ImGui::Text("FrameRate:- %d", val);
		}
		guiWindow.EndGUI();

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
		// call events
		gui.RenderGUI();
		renderer.SwapBuffers();
	}
	// terminate program
	gui.TerminateGUI();
	renderer.TerminateGLFW();
	return 0;
}