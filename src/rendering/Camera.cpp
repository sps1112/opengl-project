#include <rendering/Camera.h>

Camera::Camera(Vec3 position, Vec3 up, float yaw, float pitch) : Front(Vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(Vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = Vec3(posX, posY, posZ);
    WorldUp = Vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

Mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
    {
        Position += Front * velocity;
    }
    if (direction == BACKWARD)
    {
        Position += -Front * velocity;
    }
    if (direction == LEFT)
    {
        Position += -Right * velocity;
    }
    if (direction == RIGHT)
    {
        Position += Right * velocity;
    }
    if (direction == UP)
    {
        Position += Up * velocity;
    }
    if (direction == DOWN)
    {
        Position += -Up * velocity;
    }
    // Position.y = 0.0f; //for FPS CAMERA
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, float deltaTime, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity * deltaTime * 60;
    yoffset *= MouseSensitivity * deltaTime * 60;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch)
    {
        if (Pitch > 89.0f)
        {
            Pitch = 89.0f;
        }
        if (Pitch < -89.0f)
        {
            Pitch = -89.0f;
        }
    }
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < MINZOOM)
    {
        Zoom = MINZOOM;
    }
    if (Zoom > MAXZOOM)
    {
        Zoom = MAXZOOM;
    }
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}
