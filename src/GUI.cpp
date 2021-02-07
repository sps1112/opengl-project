#include <GUI.h>

GUI::GUI(GLFWwindow *window, int major, int minor)
{
    std::string versionText = "#version " + std::to_string(major) + std::to_string(minor) + "0";
    const char *glsl_version = versionText.c_str();
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
}

void GUI::TerminateGUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::NewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GUI::RenderGUI()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

GUIWindow::GUIWindow(std::string title)
{
    guiTitle = title;
    guiCount = 0;
}

void GUIWindow::ShowGUI()
{
    ImGui::Begin(guiTitle.c_str());
    ShowItems();
}

void GUIWindow::EndGUI()
{
    ImGui::End();
}

void GUIWindow::AddGUI(GUI_TYPE itemType, std::string itemText, bool isOnNewLine)
{
    guiCount++;
    GUIItem item(itemType, itemText, isOnNewLine);
    items.push_back(item);
}

void GUIWindow::AddGUI(GUI_TYPE itemType, std::string itemText, bool isOnNewLine, bool *pointer)
{
    guiCount++;
    GUIItem item(itemType, itemText, isOnNewLine, pointer);
    items.push_back(item);
}

void GUIWindow::AddGUI(GUI_TYPE itemType, std::string itemText, bool isOnNewLine, float *pointer, float minVal, float maxVal)
{
    guiCount++;
    GUIItem item(itemType, itemText, isOnNewLine, pointer, minVal, maxVal);
    items.push_back(item);
}

void GUIWindow::AddGUI(GUI_TYPE itemType, std::string itemText, bool isOnNewLine, bool isColor, ImVec4 *pointer)
{
    guiCount++;
    GUIItem item(itemType, itemText, isOnNewLine, isColor, pointer);
    items.push_back(item);
}

void GUIWindow::ShowItems()
{
    for (int i = 0; i < guiCount; i++)
    {
        GUIItem item = items.at(i);
        if (!item.isOnNewLine)
        {
            ImGui::SameLine();
        }
        if (item.type == GUI_LINE)
        {
            ImGui::Text((item.text).c_str());
        }
        else if (item.type == GUI_CHECKBOX)
        {
            ImGui::Checkbox((item.text).c_str(), item.pointerB);
        }
        else if (item.type == GUI_FLOAT)
        {
            ImGui::SliderFloat((item.text).c_str(), item.pointerF, item.minVal, item.maxVal);
        }
        else if (item.type == GUI_COLOR)
        {
            ImGui::ColorEdit3((item.text).c_str(), item.pointerF);
        }
    }
}

GUIItem::GUIItem(GUI_TYPE itemType, std::string itemText, bool isOnNewLine)
{
    this->type = itemType;
    this->text = itemText;
    this->isOnNewLine = isOnNewLine;
}
GUIItem::GUIItem(GUI_TYPE itemType, std::string itemText, bool isOnNewLine, bool *pointer)
{
    this->type = itemType;
    this->text = itemText;
    this->isOnNewLine = isOnNewLine;
    pointerB = pointer;
}
GUIItem::GUIItem(GUI_TYPE itemType, std::string itemText, bool isOnNewLine, float *pointer, float minVal, float maxVal)
{
    this->type = itemType;
    this->text = itemText;
    this->isOnNewLine = isOnNewLine;
    pointerF = pointer;
    this->minVal = minVal;
    this->maxVal = maxVal;
}
GUIItem::GUIItem(GUI_TYPE itemType, std::string itemText, bool isOnNewLine, bool isColor, ImVec4 *pointer)
{
    this->type = itemType;
    this->text = itemText;
    this->isOnNewLine = isOnNewLine;
    pointerF = (float *)pointer;
}