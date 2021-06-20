#include <gui/GUIWidgets.h>

char *texComboItems[] = {
    "None",
    "awesomeface",
    "wall",
    "brickwall",
    "brickwall_normal",
    "brickwall2",
    "brickwall2_normal",
    "brickwall2_disp",
    "container",
    "container2",
    "container2_specular",
    "container2_specular_color",
    "marble",
    "metal",
    "wood",
    "window",
    "grass",
    "matrix"};

void ShowSimpleOverlay(bool *p_open, int sceneNumber)
{
    const float DISTANCE = 25.0f;
    static int corner = 1;
    ImGuiIO &io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    if (corner != -1)
    {
        window_flags |= ImGuiWindowFlags_NoMove;
        ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
        ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    }
    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    if (ImGui::Begin("Example: Simple overlay", p_open, window_flags))
    {
        ImGui::Text("Scene %d\n", sceneNumber);
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Custom", NULL, corner == -1))
                corner = -1;
            if (ImGui::MenuItem("Top-left", NULL, corner == 0))
                corner = 0;
            if (ImGui::MenuItem("Top-right", NULL, corner == 1))
                corner = 1;
            if (ImGui::MenuItem("Bottom-left", NULL, corner == 2))
                corner = 2;
            if (ImGui::MenuItem("Bottom-right", NULL, corner == 3))
                corner = 3;
            if (p_open && ImGui::MenuItem("Close"))
                *p_open = false;
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}

int AddActorUI(Scene *currentScene)
{
    if (ImGui::BeginMenu("Add Actor.."))
    {
        if (ImGui::MenuItem("Triangle"))
        {
            currentScene->AddActor(TRIANGLE_2D);
        }
        if (ImGui::MenuItem("Rectangle"))
        {
            currentScene->AddActor(RECTANGLE_2D);
        }
        if (ImGui::MenuItem("Cube"))
        {
            currentScene->AddActor(CUBE_3D);
        }
        if (ImGui::MenuItem("Sphere"))
        {
            currentScene->AddActor(SPHERE_MODEL);
        }
        /*if (ImGui::MenuItem("Backpack"))
			{
				loadedScenes[listIndex].AddActor("resources/models/backpack/backpack.obj", MODEL_OBJECT);
			}*/
        ImGui::EndMenu();
    }
    return (currentScene->actorCount - 1);
}

// Demonstrate create a window with multiple child windows.
void ShowAppLayout(bool *p_open, Scene *currentScene)
{
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    static int selectedIndex = 0;
    if (ImGui::Begin("SCENE ACTOR LIST", p_open, ImGuiWindowFlags_MenuBar))
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                selectedIndex = AddActorUI(currentScene);
                if (ImGui::MenuItem("Close"))
                {
                    *p_open = false;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Left
        {
            ImGui::BeginGroup();
            ImGui::BeginChild("left pane", ImVec2(150, 0), true);
            if (ImGui::Selectable("Camera", selectedIndex == 0))
            {
                selectedIndex = 0;
            }
            if (currentScene->actorCount > 0)
            {
                selectedIndex %= currentScene->actorCount;
                for (int i = 0; i < currentScene->actorCount; i++)
                {
                    char label[64];
                    sprintf(label, (currentScene->actorList[i].name).c_str());
                    if (ImGui::Selectable(label, selectedIndex == i))
                    {
                        selectedIndex = i;
                    }
                }
            }
            /*else
            {
                char label[64];
                sprintf(label, "No Object Loaded");
                ImGui::Selectable(label, false);
                selectedIndex = 0;
            }*/
            ImGui::EndChild();
            ImGui::EndGroup();
        }
        ImGui::SameLine();

        // Right
        {
            RenderActor *selectedActor;
            ImGui::BeginGroup();
            ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
            if (currentScene->actorCount > 0)
            {
                selectedActor = &(currentScene->actorList[selectedIndex]);
                ImGui::Text((selectedActor->name).c_str());
            }
            ImGui::Separator();
            if (currentScene->actorCount > 0)
            {
                ImGui::Text("DATA");
                ImGui::Text("---------------------------");
                ImGui::InputText("Name: ", &(selectedActor->name[0]), 30);
                ImGui::TextWrapped(("Path: " + selectedActor->path + "\n").c_str());
                ImGui::Checkbox("Visibility", &(selectedActor->isVisible));
                ImGui::Text("---------------------------");
                ImGui::Text("---------------------------");
                ImGui::Text("        ");
                ImGui::Text("TRANSFORM");
                ImGui::Text("---------------------------");
                ImGui::SliderFloat3("Position", &(selectedActor->transform.position.x), -5.0f, 5.0f);
                ImGui::SliderFloat3("Rotation", &(selectedActor->transform.rotation.x), -180.0f, 180.0f);
                ImGui::SliderFloat3("Scale", &(selectedActor->transform.scale.x), 0.0001f, 10.0f);
                if (ImGui::Button("Reset Transform"))
                {
                    selectedActor->transform.Reset();
                }
                ImGui::Text("---------------------------");
                ImGui::Text("---------------------------");
                ImGui::Text("        ");
                ImGui::Text("MATERIAL");
                ImGui::Text("---------------------------");
                ImGui::ColorEdit3("Material Color", &(selectedActor->mat.albedo.col.r));
                if (ImGui::Combo("Texture", &(selectedActor->mat.albedo.texID), texComboItems, 18))
                {
                    currentScene->UpdateActor(selectedActor);
                }
                ImGui::Text("---------------------------");
                ImGui::Text("---------------------------");
                ImGui::Text("        ");
            }
            ImGui::EndChild();
            ImGui::EndGroup();
        }
    }
    ImGui::End();
}
