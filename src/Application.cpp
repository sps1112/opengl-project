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

// States of the Application
enum APP_MODE
{
	SCENE_EMPTY,
	SCENE_NORMAL,
};
APP_MODE currentMode = SCENE_EMPTY;

// UI booleans
bool lockFramerate = true;
bool showFramerate = false;
bool showDemoUI = false;
bool overlayOpen = true;
bool listOpen = true;

// Method Declarations

// Sets up the Renderer
int setup_renderer();
// Draws the Application
void draw_app();
// Draws a Normal Scene
void draw_normal_scene();
// Draws an Empty Scene
void draw_empty_scene();
// Shows the Main Menu Bar
void show_mainmenu_bar();
// Shows the File Menu
void show_file_menu();
// Shows the Edit Menu
void show_edit_menu();
// Shows the View Menu
void show_view_menu();

// Main Function
int main()
{
	if (setup_renderer() == -1)
	{
		renderer.TerminateGLFW();
		return -1;
	}
	draw_app();
	renderer.TerminateGLFW();
	return 0;
}

int setup_renderer()
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

void draw_app()
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
		case SCENE_EMPTY:
			draw_empty_scene();
			break;
		case SCENE_NORMAL:
			draw_normal_scene();
			break;
		}

		// Render GUI
		show_mainmenu_bar();

		// End Frame
		gui.RenderGUI();
		renderer.SwapBuffers(lockFramerate);
	}
	// Free Data

	// Free frame buffer
	renderer.frameBuffer.FreeFBO();
	renderer.frameBuffer.FreeRBO();

	// Terminate Program
	gui.TerminateGUI();
}

void draw_empty_scene()
{
	// Process Data
	renderer.ProcessInput();
	// Clear Screen
	renderer.SetColor(DEFAULT_BACKGROUND_COLOR.r, DEFAULT_BACKGROUND_COLOR.g, DEFAULT_BACKGROUND_COLOR.b, 1.0f);
	if (showFramerate)
	{
		ShowSimpleOverlay(&showFramerate, "FrameRate: " + std::to_string((int)(1.0f / renderer.deltaTime)), 3);
	}
}

void draw_normal_scene()
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
		ShowSimpleOverlay(&overlayOpen, ("Scene " + std::to_string(sceneNumber) + "\n"), 1);
	}
	if (listOpen)
	{
		ShowAppLayout(&listOpen, &(loadedScenes[listIndex]));
	}
	if (showFramerate)
	{
		ShowSimpleOverlay(&showFramerate, "\nFrameRate: " + std::to_string((int)(1.0f / renderer.deltaTime)), 3);
	}
}

void show_mainmenu_bar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			show_file_menu();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			show_edit_menu();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			show_view_menu();
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void show_file_menu()
{
	if (ImGui::MenuItem("New Scene")) // Adds a New Scene
	{
		currentMode = SCENE_NORMAL;
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
					currentMode = SCENE_NORMAL;
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
		currentMode = SCENE_EMPTY;
	}
	if (ImGui::MenuItem("Quit", "Escape")) // Quits Application
	{
		renderer.QuitWindow();
	}
}

void show_edit_menu()
{
	if (ImGui::BeginMenu("Edit Renderer.."))
	{
		ImGui::Checkbox("Show Framerate", &(showFramerate));
		ImGui::Checkbox("Lock Framerate", &(lockFramerate));
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Edit Current Scene.."))
	{
		if (currentMode == SCENE_NORMAL)
		{
			ImGui::ColorEdit4("Background Color", &(loadedScenes[listIndex]).data.backgroundColor.x);
			ImGui::Combo("Draw Mode", &(loadedScenes[listIndex].data.drawMode), drawComboItems, 3);
		}
		else
		{
			ImGui::MenuItem("(No Scene Open)", NULL, false, false);
		}
		ImGui::EndMenu();
	}
}

void show_view_menu()
{
	if (currentMode == SCENE_NORMAL)
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
