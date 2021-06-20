#ifndef EXAMPLE_GUI_H
#define EXAMPLE_GUI_H

#include <gui/GUI.h>
#include <object/Scene.h>
#include <gui/ObjectUI.h>

extern char *texComboItems[18];

// Shows a Simple Overlay
void ShowSimpleOverlay(bool *p_open, int sceneNumber);

int AddActorUI(Scene *currentScene);

// Demonstrate create a window with multiple child windows.
void ShowAppLayout(bool *p_open, Scene *currentScene);

#endif