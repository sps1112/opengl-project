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
	if (ImGui::BeginMenu("Edit Window"))
	{
		ImGui::EndMenu();
	}
	if (currentMode == Normal_Scene)
	{
		if (ImGui::BeginMenu("Edit Current Scene.."))
		{
			ImGui::ColorEdit4("Background Color", &(loadedScenes[listIndex]).data.backgroundColor.x);
			ImGui::Combo("Draw Mode", &drawOption, drawComboItems, 3);
			ImGui::EndMenu();
		}
		AddActorUI(&(loadedScenes[listIndex]));
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