#include <rendering/Camera.h>

Camera::Camera(Vec3 position_, Vec3 up, float yaw_, float pitch_) : front(Vec3(0.0f, 0.0f, -1.0f)), moveSpeed(CAMERA_SPEED), mouseSensitivity(CAMERA_SENSITIVITY), fovZoom(CAMERA_ZOOM)
{
    position = position_;
    worldUp = up;
    yaw = yaw_;
    pitch = pitch_;
    update_camera_vectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw_, float pitch_) : front(Vec3(0.0f, 0.0f, -1.0f)), moveSpeed(CAMERA_SPEED), mouseSensitivity(CAMERA_SENSITIVITY), fovZoom(CAMERA_ZOOM)
{
    position = Vec3(posX, posY, posZ);
    worldUp = Vec3(upX, upY, upZ);
    yaw = yaw_;
    pitch = pitch_;
    update_camera_vectors();
}

Mat4 Camera::get_view_matrix()
{
    return glm::lookAt(position, position + front, up);
}

void Camera::process_keyboard(CAMERA_MOVE_DIR direction, float deltaTime)
{
    float velocity = moveSpeed * deltaTime;
    if (direction == FORWARD_DIR)
    {
        position += front * velocity;
    }
    if (direction == BACKWARD_DIR)
    {
        position += -front * velocity;
    }
    if (direction == LEFT_DIR)
    {
        position += -right * velocity;
    }
    if (direction == RIGHT_DIR)
    {
        position += right * velocity;
    }
    if (direction == UP_DIR)
    {
        position += up * velocity;
    }
    if (direction == DOWN_DIR)
    {
        position += -up * velocity;
    }
    // position.y = 0.0f; //for FPS CAMERA
}

void Camera::process_mouse(float xoffset, float yoffset, float deltaTime, GLboolean constrainPitch)
{
    xoffset *= mouseSensitivity * deltaTime * 60;
    yoffset *= mouseSensitivity * deltaTime * 60;

    yaw += xoffset;
    pitch += yoffset;

    if (constrainPitch)
    {
        pitch = clamp(pitch, -CAMERA_MAX_PITCH, CAMERA_MAX_PITCH);
    }
    update_camera_vectors();
}

void Camera::process_scroll(float yoffset)
{
    fovZoom -= (float)yoffset;
    fovZoom = clamp(fovZoom, CAMERA_MINZOOM, CAMERA_MAXZOOM);
}

void Camera::update_camera_vectors()
{
    glm::vec3 frontLocal;
    frontLocal.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    frontLocal.y = sin(glm::radians(pitch));
    frontLocal.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(frontLocal);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}
