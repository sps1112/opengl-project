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
        if (ImGui::MenuItem("Backpack"))
        {
            currentScene->AddActor(BACKPACK_MODEL);
        }
        ImGui::EndMenu();
    }
    return (currentScene->actorCount);
}

void ShowSectionHeader(const char *title)
{
    ImGui::Separator();
    ImGui::Text(title);
    ImGui::Text("---------------------------");
}

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
        // Left Column
        {
            ImGui::BeginGroup();
            ImGui::BeginChild("left pane", ImVec2(150, 0), true);
            if (ImGui::Selectable(currentScene->cameraList[0].name.c_str(), selectedIndex == 0))
            {
                selectedIndex = 0;
            }
            if (currentScene->actorCount > 0)
            {
                selectedIndex %= (currentScene->actorCount) + 1;
                for (int i = 0; i < currentScene->actorCount; i++)
                {
                    char label[64];
                    sprintf(label, (currentScene->actorList[i].name).c_str());
                    if (ImGui::Selectable(label, selectedIndex == i + 1))
                    {
                        selectedIndex = i + 1;
                    }
                }
            }
            ImGui::EndChild();
            ImGui::EndGroup();
        }
        ImGui::SameLine();
        // Right Column
        {
            RenderActor *selectedActor;
            ImGui::BeginGroup();
            ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
            if (selectedIndex == 0)
            {
                CameraActor *cam = &(currentScene->cameraList[0]);
                ImGui::Text((cam->name).c_str());
                ShowSectionHeader("DATA");
                ImGui::InputText("Name: ", &(currentScene->cameraList[0].name[0]), 30);
                ShowSectionHeader("TRANSFORM");
                if (ImGui::SliderFloat3("Position", &(cam->transform.position.x), -5.0f, 5.0f))
                {
                    cam->RefreshCamera(true);
                }
                if (ImGui::SliderFloat3("Rotation", &(cam->transform.rotation.x), -180.0f, 180.0f))
                {
                    cam->RefreshCamera(true);
                }
                ImGui::SliderFloat3("Scale", &(cam->transform.scale.x), 0.0001f, 10.0f);
                if (ImGui::Button("Reset Transform"))
                {
                    cam->ResetTransform();
                }
                ShowSectionHeader("CAMERA");
                ImGui::Checkbox("Orthographic", &(cam->isOrtho));
                ImGui::SliderFloat("Camera Size", &(cam->camSize), 0.0f, 15.0f);
                ImGui::Checkbox("Can Move", &(cam->canMove));
                ImGui::Checkbox("Can Rotate", &(cam->canRotate));
                ImGui::SliderFloat("FOV", &(cam->GetCamera()->Zoom), CAMERA_MINZOOM, CAMERA_MAXZOOM);
            }
            else
            {
                if (currentScene->actorCount > 0)
                {
                    selectedActor = &(currentScene->actorList[selectedIndex - 1]);
                    ImGui::Text((selectedActor->name).c_str());
                }
                if (currentScene->actorCount > 0)
                {
                    ShowSectionHeader("DATA");
                    ImGui::InputText("Name: ", &(selectedActor->name[0]), 30);
                    ImGui::TextWrapped(("Path: " + selectedActor->path + "\n").c_str());
                    ImGui::Checkbox("Visibility", &(selectedActor->isVisible));
                    ShowSectionHeader("TRANSFORM");
                    ImGui::SliderFloat3("Position", &(selectedActor->transform.position.x), -5.0f, 5.0f);
                    ImGui::SliderFloat3("Rotation", &(selectedActor->transform.rotation.x), -180.0f, 180.0f);
                    ImGui::SliderFloat3("Scale", &(selectedActor->transform.scale.x), 0.0001f, 10.0f);
                    if (ImGui::Button("Reset Transform"))
                    {
                        selectedActor->transform.reset_to_origin();
                    }
                    ShowSectionHeader("MATERIAL");
                    ImGui::ColorEdit3("Material Color", &(selectedActor->mat.albedo.col.r));
                    if (ImGui::Combo("Texture", &(selectedActor->mat.albedo.texID), texComboItems, 18))
                    {
                        currentScene->UpdateActor(selectedActor);
                    }
                }
            }
            ImGui::EndChild();
            ImGui::EndGroup();
        }
    }
    ImGui::End();
}
