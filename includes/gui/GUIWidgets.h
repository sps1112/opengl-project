#ifndef GUI_WIDGETS_H
#define GUI_WIDGETS_H

#include <gui/GUI.h>
#include <object/Scene.h>
#include <gui/ObjectUI.h>

extern char *texComboItems[18];

// Shows a Simple Overlay
void ShowSimpleOverlay(bool *p_open, std::string text, int position = 1);
// Add Actor UI window
int AddActorUI(Scene *currentScene);
// Shows a section header in the UI
void ShowSectionHeader(const char *title);
// Demonstrate create a window with multiple child windows.
void ShowAppLayout(bool *p_open, Scene *currentScene);

#endif