#ifndef UTILS_H
#define UTILS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <iostream>
#include <string>

unsigned int LoadTexture(const char *path);
void Log(const char *message);
void Log(std::string message);

#endif