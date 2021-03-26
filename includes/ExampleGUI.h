#ifndef EXAMPLE_GUI_H
#define EXAMPLE_GUI_H

#include <GUI.h>
#include <Scene.h>
#include <ObjectUI.h>

// Shows a Simple Overlay
void ShowSimpleOverlay(bool *p_open, int sceneNumber);

// Demonstrate create a window with multiple child windows.
void ShowAppLayout(bool *p_open, Scene *currentScene);

// Shows Object Properties to be edited
void ShowPropertyEditor(bool *p_open, SceneObject *object);

#endif