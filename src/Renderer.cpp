#include <Renderer.h>

static RenderCamera rCamera;

Renderer::Renderer(int majorVersion, int minorVersion, int windowWidth, int windowHeight)
{
    major = majorVersion;
    minor = minorVersion;
    width = windowWidth;
    height = windowHeight;
}

void Renderer::SetupGLFW()
{
    // glfw initialise
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);             // open gl version 3.x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);             // version is 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // use core profile
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for Mac
}

void Renderer::TerminateGLFW()
{
    glfwTerminate();
}

void Renderer::CreateWindow(const char *title, GLFWmonitor *monitor, GLFWwindow *share)
{
    window = glfwCreateWindow(width, height, title, monitor, share);
    if (window == NULL)
    {
        Log("Failed to create GLFW window");
    }
}

void Renderer::SetData()
{
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    // glfwSetWindowAspectRatio(window, 16, 9);
    // glfwSetWindowSizeLimits(window, 160, 90, 1600, 900);
}

void Renderer::SetOtherData()
{
    glEnable(GL_DEPTH_TEST);                           // Enable Z buffering
    glDepthFunc(GL_LESS);                              // Closer objects will be drawn in front
    glEnable(GL_STENCIL_TEST);                         // Enables editing Stencil
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);               // if (n!=1) ==> (n=1) =>Draw Everything
    glEnable(GL_BLEND);                                // Enables Blend test
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set Blending mode as {x*a+y*(1-a)}
    stbi_set_flip_vertically_on_load(true);            // Set before loading model
    glEnable(GL_CULL_FACE);                            // Enable Face Culling
    glCullFace(GL_BACK);                               // To Cull Back Faces (do no draw)
    glFrontFace(GL_CCW);                               // Front faces are those with CCW motion
}

int Renderer::CheckGLAD()
{
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (!status)
    {
        Log("Failed to intialize GLAD");
    }
    return status;
}

int Renderer::CheckWindowFlag()
{
    int status = glfwWindowShouldClose(window);
    return status;
}

void Renderer::SwapBuffers(bool lockFramerate)
{
    glfwSwapBuffers(window);
    glfwPollEvents();
    if (!lockFramerate)
    {
        glfwSwapInterval(0);
    }
    else
    {
        glfwSwapInterval(1);
    }
}

void Renderer::StartTimer()
{
    currentFrameTime = glfwGetTime();
    prevFrameTime = currentFrameTime;
    timePeriod = 100.0f;
    deltaTime = 0.0f;
}

void Renderer::NewFrame()
{
    currentFrameTime = glfwGetTime();
    deltaTime = currentFrameTime - prevFrameTime;
    prevFrameTime = currentFrameTime;
}

void Renderer::SetCamera(Camera camera)
{
    rCamera.camera = Camera(camera.Position);
    rCamera.lastX = width / 2.0f;
    rCamera.lastY = height / 2.0f;
    rCamera.isFirstMouse = true;
    rCamera.xOff = 0;
    rCamera.yOff = 0;
}

float Renderer::GetZoom()
{
    return rCamera.camera.Zoom;
}

Camera *Renderer::GetCamera()
{
    return &(rCamera.camera);
}

void Renderer::SetCursor(bool status)
{
    if (status)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // enable cursor
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // disable cursor
    }
}

// Sets color of background
void Renderer::SetColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a); // Grey
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

bool Renderer::CheckInput(int key)
{
    return (glfwGetKey(window, key) == GLFW_PRESS);
}

// check exit input
void Renderer::ProcessInput(bool moveStatus)
{
    if (CheckInput(GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (moveStatus)
    {
        if (CheckInput(GLFW_KEY_W))
        {
            rCamera.camera.ProcessKeyboard(FORWARD, deltaTime);
        }
        if (CheckInput(GLFW_KEY_S))
        {
            rCamera.camera.ProcessKeyboard(BACKWARD, deltaTime);
        }
        if (CheckInput(GLFW_KEY_A))
        {
            rCamera.camera.ProcessKeyboard(LEFT, deltaTime);
        }
        if (CheckInput(GLFW_KEY_D))
        {
            rCamera.camera.ProcessKeyboard(RIGHT, deltaTime);
        }
        if (CheckInput(GLFW_KEY_UP))
        {
            rCamera.camera.ProcessKeyboard(UP, deltaTime);
        }
        if (CheckInput(GLFW_KEY_DOWN))
        {
            rCamera.camera.ProcessKeyboard(DOWN, deltaTime);
        }
    }
}

void Renderer::ProcessMouse(bool rotateStatus)
{
    if (rotateStatus)
    {
        SetCursor(false);
        rCamera.camera.ProcessMouseMovement(rCamera.xOff, rCamera.yOff, deltaTime);
    }
    else
    {
        SetCursor(true);
    }
    rCamera.xOff = 0;
    rCamera.yOff = 0;
}

void Renderer::SetDraw(int choice)
{
    unsigned int fillMode = GL_FILL;
    if (choice == 0)
    {
        fillMode = GL_LINE; // wireframe
    }
    else if (choice == 1)
    {
        fillMode = GL_POINT; // wireframe
    }
    glPolygonMode(GL_FRONT_AND_BACK, fillMode);
}

float Renderer::GetCurrentWidth()
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    return width;
}

float Renderer::GetCurrentHeight()
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    return height;
}

void VertexArray::GenerateBuffers()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}

void VertexArray::BindVAO()
{
    glBindVertexArray(VAO);
}

void VertexArray::UnBindVAO()
{
    glBindVertexArray(0);
}

void VertexArray::BindVBO(int vertexCount, GLsizeiptr strideSize, void *pointer)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * strideSize, pointer, GL_STATIC_DRAW);
}
void VertexArray::BindEBO(int indicesCount, void *pointer)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(unsigned int), pointer, GL_STATIC_DRAW);
}

void VertexArray::UnBindVBO()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexArray::UnBindEBO()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VertexArray::SetAttribArray(int layoutLayer, int count, GLsizeiptr strideSize, const void *pointer)
{
    glEnableVertexAttribArray(layoutLayer);
    glVertexAttribPointer(layoutLayer, count, GL_FLOAT, GL_FALSE, strideSize, pointer);
}

void VertexArray::DrawTriangles(int vertexCount, int startIndex)
{
    BindVAO();
    glDrawArrays(GL_TRIANGLES, startIndex, vertexCount);
    UnBindVAO();
}

void VertexArray::DrawElements(int indicesCount)
{
    BindVAO();
    glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
    UnBindVAO();
}

void VertexArray::FreeData()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

// callback on  window size change
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// callback for mouse movement
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (rCamera.isFirstMouse)
    {
        rCamera.isFirstMouse = false;
        rCamera.lastX = xpos;
        rCamera.lastY = ypos;
    }
    float xOffset = xpos - rCamera.lastX;
    float yOffset = rCamera.lastY - ypos; // y is reversed
    rCamera.lastX = xpos;
    rCamera.lastY = ypos;
    rCamera.xOff = xOffset;
    rCamera.yOff = yOffset;
}

// callback for the mouse scroll wheel
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    rCamera.camera.ProcessMouseScroll(yoffset);
}