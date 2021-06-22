// Custom Headers
#include <config.h>				// Configuration Header
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
#include <gui/GUIWidgets.h>		// Custom GUI Widgets
#include <object/Actor.h>		// Actor Header

// Standard Headers
#include <iostream>
#include <vector>

// Renderer Settings
Renderer renderer(OPENGL_MAJOR_VERSION, OPENGL_MINOR_VERSION, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

// Scene Settings
Scene *loadedScenes;
const int maxSceneCount = MAX_LOADED_SCENES_COUNT; // Max Number of Scene loaded
int currentSceneIndex = 0;						   // Index of  Current Scene in UI
int loadIndex = -1;								   // Index of last loaded Scene in Array
int loadedSceneCount = 0;						   // Number of last loaded Scene
int listIndex;									   // Index of Current Loaded Scene in Array

// The States of the Application
enum AppMode
{
	Empty_Scene,
	Normal_Scene,
};
AppMode currentMode = Empty_Scene;

// UI booleans
bool showDemoUI = false;
bool overlayOpen = true;
bool listOpen = true;

// Method Declarations
int SetupRenderer();
void Draw();
void DrawNormalScene();
void DrawEmptyScene();
void ShowMainMenuBar();
void ShowFileMenu();
void ShowEditMenu();
void ShowViewMenu();

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
	renderer.CreateWindow(DEFAULT_WINDOW_TITLE);
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
	GUI gui(renderer.window, OPENGL_MAJOR_VERSION, OPENGL_MINOR_VERSION);
	loadedScenes = new Scene[maxSceneCount];

	// Start Render Loop
	renderer.StartTimer();
	/*
	Render Loop
	1. New Frame :- Application
	2. Process Scene:- Application
		a. Get New Data :- Scene
		b. Process Data :- Scene
		c. Refresh Last Frame :- Scene
			i. Clear Screen
			ii. Bind FBO
		d. Render Objects :- Scene
			i. Draw Opaque Objects
				i. Ground First
				ii. Others Second
			ii. Draw Translucent Objects
				i. Farthest to Closest
			iii. Draw Skybox
			iv. Draw 2D objects
		e. Render FBO :- Scene
			i. Unbind FBO
			ii. Clear Screen
			iii. Draw Texture
	8. Render GUI :- Application
	9. End Frame :- Applicarion
	*/
	while (!renderer.CheckWindowFlag())
	{
		// New Frame
		gui.NewFrame();
		renderer.NewFrame();

		// Process Scene
		switch (currentMode)
		{
		case Empty_Scene:
			DrawEmptyScene();
			break;
		case Normal_Scene:
			DrawNormalScene();
			break;
		}

		// Render GUI
		ShowMainMenuBar();

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

void DrawEmptyScene()
{
	// Process Data
	renderer.ProcessInput();
	// Clear Screen
	renderer.SetColor(DEFAULT_BACKGROUND_COLOR.r, DEFAULT_BACKGROUND_COLOR.g, DEFAULT_BACKGROUND_COLOR.b, 1.0f);
}

void DrawNormalScene()
{
	// Draw Scene
	int sceneNumber = max(1, loadedSceneCount - (maxSceneCount - 1)) + currentSceneIndex;
	listIndex = (sceneNumber + maxSceneCount - 1) % maxSceneCount;
	loadedScenes[listIndex].DrawScene(renderer);

	// Render GUI
	if (showDemoUI)
	{
		ImGui::ShowDemoWindow();
	}
	if (overlayOpen)
	{
		ShowSimpleOverlay(&overlayOpen, sceneNumber);
	}
	if (listOpen)
	{
		ShowAppLayout(&listOpen, &(loadedScenes[listIndex]));
	}
}

void ShowMainMenuBar()
{
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
		currentSceneIndex = (loadedSceneCount >= maxSceneCount) ? (maxSceneCount - 1) : loadIndex;
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
			int startIndex = max(1, loadedSceneCount - (maxSceneCount - 1));
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
			ImGui::ColorEdit4("Background Color", &(loadedScenes[listIndex]).data.backgroundColor.x);
			ImGui::Combo("Draw Mode", &(loadedScenes[listIndex].data.drawMode), drawComboItems, 3);
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
			ImGui::MenuItem("Demo UI", NULL, &showDemoUI);
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
