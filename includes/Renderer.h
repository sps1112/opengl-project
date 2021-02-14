#ifndef RENDERER_H
#define RENDERER_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

// glm libraries for math
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <Camera.h>
#include <Utils.h>
#include <CustomMath.h>
#include <Texture.h>

struct RenderCamera
{
    Camera camera;
    float lastX;
    float lastY;
    bool isFirstMouse;
    float xOff;
    float yOff;
};

class VertexArray
{
public:
    unsigned int VAO, VBO, EBO;
    void GenerateBuffers();
    void BindVAO();
    void UnBindVAO();
    void UnBindVBO();
    void UnBindEBO();
    void BindVBO(int vertexCount, GLsizeiptr strideSize, void *pointer);
    void BindEBO(int indicesCount, void *pointer);
    void SetAttribArray(int layoutLayer, int count, GLsizeiptr strideSize, const void *pointer = (void *)0);
    void DrawTriangles(int vertexCount, int startIndex);
    void DrawElements(int indicesCount);
    void FreeData();

private:
};

class FrameBuffer
{
public:
    unsigned int FBO;
    unsigned int textureColorBuffer;
    unsigned int RBO;
    FrameBuffer();
    void GenerateFBO();
    void BindFBO();
    void UnBindFBO();
    void FreeFBO();
    void GenerateRBO();
    void BindRBO();
    void UnBindRBO();
    void FreeRBO();
    void RefreshTexture(int width, int height);
    void RefreshRBO(int width, int height);
    void AttachTexture();
    void AttachRBO();
    void CheckStatus();

private:
};

class Renderer
{
public:
    int major;
    int minor;
    int width;
    int height;
    FrameBuffer frameBuffer;
    GLFWwindow *window;
    float deltaTime;
    Renderer(int majorVersion, int minorVersion, int windowWidth = 800, int windowHeight = 600);
    void SetupGLFW();
    void TerminateGLFW();
    void CreateWindow(const char *title, GLFWmonitor *monitor = NULL, GLFWwindow *share = NULL);
    void SetData();
    void SetOtherData();
    int CheckGLAD();
    void SetupFrameBuffer();
    int CheckWindowFlag();
    void SwapBuffers(bool lockFramerate = true);
    void StartTimer();
    void NewFrame();
    void SetCamera(Camera camera);
    float GetZoom();
    Camera *GetCamera();
    void SetCursor(bool status);
    void SetColor(float r, float g, float b, float a = 1.0f);
    bool CheckInput(int key);
    void ProcessInput(bool moveStatus = false);
    void ProcessMouse(bool rotateStatus);
    void SetDraw(int choice = 2);
    float GetCurrentWidth();
    float GetCurrentHeight();

private:
    float currentFrameTime;
    float timePeriod;
    float prevFrameTime;
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
#endif