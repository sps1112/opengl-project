#ifndef GUI_H
#define GUI_H
// imgui headers
#include <external/imgui/imgui.h>
#include <external/imgui/imgui_impl_glfw.h>
#include <external/imgui/imgui_impl_opengl3.h>

#include <rendering/Renderer.h>
#include <utility/Utils.h>

#include <iostream>
#include <vector>

using namespace std;

enum GUI_TYPE
{
    GUI_LINE,
    GUI_CHECKBOX,
    GUI_FLOAT,
    GUI_COLOR,
    GUI_VECTOR3,
    GUI_COMBO,
};

class GUIItem
{
public:
    GUI_TYPE type;
    std::string text;
    bool isOnNewLine;
    bool *pointerB;
    float *pointerF;
    int *pointerI;
    float minVal;
    float maxVal;
    int count;
    std::string *choices;
    GUIItem(GUI_TYPE itemType, std::string itemText, bool isOnNewLine);
    GUIItem(GUI_TYPE itemType, std::string itemText, bool isOnNewLine, bool *pointer);
    GUIItem(GUI_TYPE itemType, std::string itemText, bool isOnNewLine, float *pointer, float minVal = -10000, float maxVal = 10000);
    GUIItem(GUI_TYPE itemType, std::string itemText, bool isOnNewLine, bool isColor, ImVec4 *pointer);
    GUIItem(GUI_TYPE itemType, std::string itemText, bool isOnNewLine, bool isCombo, int *pointer, const char *choices[], int count);

private:
};

class GUI
{
public:
    GUI(GLFWwindow *window, int major, int minor);
    void TerminateGUI();
    void NewFrame();
    void RenderGUI();

private:
};

class GUIWindow
{
public:
    std::string guiTitle;
    int guiCount;
    GUIWindow(std::string title);
    void ShowGUI();
    void EndGUI();
    void AddGUI(GUI_TYPE itemType, std::string itemText, bool isOnNewLine);
    void AddGUI(GUI_TYPE itemType, std::string itemText, bool isOnNewLine, bool *pointer);
    void AddGUI(GUI_TYPE itemType, std::string itemText, bool isOnNewLine, float *pointer, float minVal = -10000, float maxVal = 10000);
    void AddGUI(GUI_TYPE itemType, std::string itemText, bool isOnNewLine, bool isColor, ImVec4 *pointer);
    void AddGUI(GUI_TYPE itemType, std::string itemText, bool isOnNewLine, bool isCombo, int *pointer, const char *choices[], int count);

private:
    vector<GUIItem> items;
    void ShowItems();
};

#endif