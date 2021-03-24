#ifndef EXAMPLE_GUI_H
#define EXAMPLE_GUI_H
#include <GUI.h>
#include <Scene.h>

static void ShowSimpleOverlay(bool *p_open, int sceneNumber)
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

// Demonstrate create a window with multiple child windows.
static void ShowAppLayout(bool *p_open, Scene *currentScene)
{
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Scene Object List", p_open, ImGuiWindowFlags_MenuBar))
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Close"))
                    *p_open = false;
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Left
        static int selectedIndexA = 0;
        static int selectedIndexB = 0;
        static bool objectSelected = true;
        {
            ImGui::BeginGroup();
            ImGui::BeginChild("left pane", ImVec2(150, 0), true);
            if (ImGui::BeginTabBar("##ObjectTabs", ImGuiTabBarFlags_None))
            {
                if (ImGui::BeginTabItem("Objects"))
                {
                    objectSelected = true;
                    if (currentScene->objectCount > 0)
                    {
                        selectedIndexA %= currentScene->objectCount;
                        for (int i = 0; i < currentScene->objectCount; i++)
                        {
                            char label[64];
                            sprintf(label, (currentScene->objects[i].name).c_str());
                            if (ImGui::Selectable(label, selectedIndexA == i))
                                selectedIndexA = i;
                        }
                    }
                    else
                    {
                        char label[64];
                        sprintf(label, "No Object Loaded");
                        ImGui::Selectable(label, false);
                        selectedIndexA = 0;
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Uniques"))
                {
                    objectSelected = false;
                    if (currentScene->uniqueCount > 0)
                    {
                        selectedIndexB %= currentScene->uniqueCount;
                        for (int i = 0; i < currentScene->uniqueCount; i++)
                        {
                            char label[64];
                            sprintf(label, (currentScene->uniques[i].name).c_str());
                            if (ImGui::Selectable(label, selectedIndexB == i))
                                selectedIndexB = i;
                        }
                    }
                    else
                    {
                        char label[64];
                        sprintf(label, "No Unique Loaded");
                        ImGui::Selectable(label, false);
                        selectedIndexB = 0;
                    }
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::EndChild();
            ImGui::EndGroup();
        }
        ImGui::SameLine();

        // Right
        {
            SceneObject *selectedObject;
            UniqueObject *selectedUnique;
            ImGui::BeginGroup();
            ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
            if (objectSelected)
            {
                if (currentScene->objectCount > 0)
                {
                    selectedObject = &(currentScene->objects[selectedIndexA]);
                    ImGui::Text((selectedObject->name).c_str());
                }
            }
            else
            {
                if (currentScene->uniqueCount > 0)
                {
                    selectedUnique = &(currentScene->uniques[selectedIndexB]);
                    ImGui::Text((selectedUnique->name).c_str());
                }
            }
            ImGui::Separator();
            if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
            {
                if (ImGui::BeginTabItem("Details"))
                {
                    if (objectSelected)
                    {
                        if (currentScene->objectCount > 0)
                        {
                            ImGui::TextWrapped(("Name: " + selectedObject->name +
                                                "\nPath: " + selectedObject->path + "\n")
                                                   .c_str());
                        }
                    }
                    else
                    {
                        if (currentScene->uniqueCount > 0)
                        {
                            ImGui::TextWrapped(("Name: " + selectedUnique->name +
                                                "\nPath: " + selectedUnique->path + "\n")
                                                   .c_str());
                        }
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Other"))
                {
                    if (objectSelected)
                    {
                        if (currentScene->objectCount > 0)
                        {
                            ImGui::TextWrapped(("Object Type: " + std::to_string(selectedObject->objectType) +
                                                "\nUnique Name: " + (currentScene->GetUnique(selectedObject->path))->name +
                                                "\nPath: " + selectedObject->path)
                                                   .c_str());
                        }
                    }
                    else
                    {
                        if (currentScene->uniqueCount > 0)
                        {
                            ImGui::TextWrapped(("Object Type: " + std::to_string(selectedUnique->objectType) +
                                                "\nCount: " + std::to_string(selectedUnique->count))
                                                   .c_str());
                        }
                    }
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::EndChild();
            ImGui::EndGroup();
        }
    }
    ImGui::End();
}

#endif