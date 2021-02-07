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
#include <Math.h>

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
    void BindVBO(int vertexCount, GLsizeiptr size, void *pointer);
    void BindEBO(int indicesCount, void *pointer);
    void SetAttribArray(int layoutLayer, int count, GLsizeiptr size, const void *pointer);
    void DrawTriangles(int vertexCount, int startIndex);
    void DrawElements(int indicesCount);

private:
};

class Renderer
{
public:
    int major;
    int minor;
    int width;
    int height;
    GLFWwindow *window;
    Renderer(int majorVersion, int minorVersion, int windowWidth, int windowHeight);
    void SetupGLFW();
    void TerminateGLFW();
    void CreateWindow(const char *title, GLFWmonitor *monitor = NULL, GLFWwindow *share = NULL);
    void SetData();
    void SetOtherData();
    void SetCamera(Camera camera);
    float GetZoom();
    Camera *GetCamera();
    int checkGLAD();
    void SetCursor(bool status);
    void SetColor(float r, float g, float b, float a);
    bool CheckInput(int key);
    void ProcessInput(bool moveStatus, float delta);
    void ProcessMouse(bool rotateStatus, float delta);
    void SetDraw(int choice);
    void ProcessDraw(bool lineStatus, bool pointStatus, bool fillStatus);

private:
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
#endif