// Custom Headers
#include <rendering/Renderer.h> // Renderer header
#include <utility/Utils.h>		// Utility header
#include <gui/GUI.h>			// GUI header
#include <object/Scene.h>		// Scene header
#include <utility/CustomMath.h> // Math header
#include <utility/FileSystem.h> // Filesystem header
#include <object/Primitive.h>	// Primitive header
#include <rendering/Shader.h>	// Shader header
#include <rendering/Texture.h>	// Texture header
#include <rendering/Camera.h>	// Camera header
#include <rendering/Light.h>	// Light header
#include <object/Model.h>		// Model header
#include <gui/ExampleGUI.h>		// Custom GUI Widgets

// Standard Headers
#include <iostream>
#include <vector>

// Renderer Settings
const int majorVersion = 4;
const int minorVersion = 6;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char *windowTitle = "OpenGL window";

Renderer renderer(majorVersion, minorVersion, SCR_WIDTH, SCR_HEIGHT);

// Scene Settings
Scene *loadedScenes;
const int maxSceneCount = 3; // Max Number of Scene loaded
int currentSceneIndex = 0;	 // Index of  Current Scene in UI
int loadIndex = -1;			 // Index of last loaded Scene in Array
int loadedSceneCount = 0;	 // Number of last loaded Scene
int listIndex;				 // Index of Current Loaded Scene in Array

enum AppMode
{
	Empty_Scene,
	Normal_Scene,
};
AppMode currentMode = Empty_Scene;

// UI booleans
bool overlayOpen = true;
bool listOpen = false;

// Method Declarations
int SetupRenderer();
void Draw();
void ClearScreen();
void ShowFileMenu();
void ShowEditMenu();
void ShowViewMenu();
void DrawNormalScene();

// Project Settings
int drawOption = 2;
const char *drawComboItems[] = {
	"Draw Line", "Draw Point", "Draw Fill"};

// Main Function
int main()
{
	if (SetupRenderer() == -1)
	{
		renderer.TerminateGLFW();
		return -1;
	}
	Draw();
	renderer.TerminateGLFW();
	return 0;
}

// Setup Renderer Data
int SetupRenderer()
{
	renderer.SetupGLFW();
	renderer.CreateWindow(windowTitle);
	if (renderer.window == NULL)
	{
		return -1;
	}
	renderer.SetData();
	if (!renderer.CheckGLAD())
	{
		return -1;
	}
	renderer.SetOtherData();
	renderer.SetupFrameBuffer();
	return 0;
}

// Draw Method
void Draw()
{
	// Setup GUI
	GUI gui(renderer.window, majorVersion, minorVersion);
	loadedScenes = new Scene[maxSceneCount];

	// Start Render Loop
	renderer.StartTimer();
	/*
	Render Loop
	1. New Frame
	2. Process Data
	3. Get New Data
	4. Refresh Last Frame
		a. Clear Screen
		b. Bind FBO
	5. Render Objects
		a. Draw Opaque Objects
			i. Ground First
			ii. Others Second
		b. Draw Translucent Objects
			i. Farthest to Closest
		c. Draw Skybox
		d. Draw 2D objects
	6. Render FBO
		a. Unbind FBO
		b. Clear Screen
		c. Draw Texture
	7. Render GUI
	8. End Frame
	*/
	while (!renderer.CheckWindowFlag())
	{
		// New Frame
		gui.NewFrame();
		renderer.NewFrame();

		// Process Data
		renderer.ProcessInput();
		renderer.SetDraw(drawOption);

		// Get New Data
		/*int currentWidth = (int)renderer.GetCurrentWidth();
		int currentHeight = (int)renderer.GetCurrentHeight();*/

		// Refresh Frame
		switch (currentMode)
		{
		case Empty_Scene:
			ClearScreen();
			break;
		case Normal_Scene:
			DrawNormalScene();
			break;
		}

		// Render GUI
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ShowFileMenu();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				ShowEditMenu();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View"))
			{
				ShowViewMenu();
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		// End Frame
		gui.RenderGUI();
		renderer.SwapBuffers();
	}
	// Free Data
	// Free frame buffer
	renderer.frameBuffer.FreeFBO();
	renderer.frameBuffer.FreeRBO();

	// Terminate Program
	gui.TerminateGUI();
}

void ClearScreen()
{
	renderer.SetColor(0.0f, 0.0, 0.0f, 1.0f);
}

void DrawNormalScene()
{
	// Draw Scene
	int sceneNumber = max(1, loadedSceneCount - 2) + currentSceneIndex;
	listIndex = (sceneNumber + maxSceneCount - 1) % maxSceneCount;
	loadedScenes[listIndex].DrawScene(renderer);

	// Render GUI
	ImGui::ShowDemoWindow();
	if (overlayOpen)
	{
		ShowSimpleOverlay(&overlayOpen, sceneNumber);
	}
	if (listOpen)
	{
		ShowAppLayout(&listOpen, &(loadedScenes[listIndex]));
	}
}

void ShowFileMenu()
{
	if (ImGui::MenuItem("New Scene")) // Add New Scene
	{
		currentMode = Normal_Scene;
		Scene nextScene("New Scene");
		loadIndex = (loadIndex + 1) % maxSceneCount;
		loadedSceneCount++;
		loadedScenes[loadIndex] = nextScene;
		currentSceneIndex = (loadedSceneCount >= maxSceneCount) ? 2 : loadIndex;
	}
	ImGui::Separator();
	if (ImGui::MenuItem("Open Scene")) // Load a Template Scene
	{
	}
	if (ImGui::BeginMenu("Open Recent Scene")) // Load Recently Open Scenes
	{
		if (loadedSceneCount == 0)
		{
			ImGui::MenuItem("(No Loaded Scenes)", NULL, false, false);
		}
		else
		{
			int startIndex = max(1, loadedSceneCount - 2);
			for (int i = startIndex; i <= loadedSceneCount; i++)
			{
				std::string name = "Scene " + std::to_string(i);
				if (ImGui::MenuItem(name.c_str()))
				{
					currentMode = Normal_Scene;
					currentSceneIndex = (i - startIndex) % maxSceneCount;
				}
			}
		}
		ImGui::EndMenu();
	}
	ImGui::Separator();
	if (ImGui::MenuItem("Save Scene")) // Save the Current Scene as a File
	{
	}
	ImGui::Separator();
	if (ImGui::MenuItem("Close Scene")) // Close Current Scene
	{
		currentMode = Empty_Scene;
	}
	if (ImGui::MenuItem("Quit", "Escape"))
	{
		renderer.QuitWindow();
	}
}

void ShowEditMenu()
{
	if (currentMode == Normal_Scene)
	{
		if (ImGui::BeginMenu("Edit Current Scene.."))
		{
			ImGui::ColorEdit4("Background Color", &(loadedScenes[listIndex]).backgroundColor.x);
			ImGui::Combo("Draw Mode", &drawOption, drawComboItems, 3);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Add Object.."))
		{
			if (ImGui::MenuItem("Triangle"))
			{
				loadedScenes[listIndex].AddObject("resources/primitives/2D/triangle.2d", PRIMITIVE_OBJECT);
			}
			if (ImGui::MenuItem("Rectangle"))
			{
				loadedScenes[listIndex].AddObject("resources/primitives/2D/rectangle.2d", PRIMITIVE_OBJECT);
			}
			if (ImGui::MenuItem("Cube"))
			{
				loadedScenes[listIndex].AddObject("resources/primitives/3D/cube.3d", PRIMITIVE_OBJECT);
			}
			/*if (ImGui::MenuItem("Backpack"))
			{
				loadedScenes[listIndex].AddObject("resources/models/backpack/backpack.obj", MODEL_OBJECT);
			}*/
			ImGui::EndMenu();
		}
	}
	else
	{
		ImGui::MenuItem("(No Scene Open)", NULL, false, false);
	}
}

void ShowViewMenu()
{
	if (currentMode == Normal_Scene)
	{
		if (ImGui::BeginMenu("SetView"))
		{
			ImGui::MenuItem("Overlay", NULL, &overlayOpen);
			ImGui::MenuItem("Object List", NULL, &listOpen);
			ImGui::EndMenu();
		}
	}
	else
	{
		ImGui::MenuItem("(No Scene Open)", NULL, false, false);
	}
}