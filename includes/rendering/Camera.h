#ifndef CAMERA_H
#define CAMERA_H

// Header declarations
#include <external/glad/glad.h>
#include <utility/CustomMath.h>
#include <config.h>

#include <vector>
#include <iostream>

// Camera movement directions
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// Default values
const float YAW = CAMERA_YAW;
const float PITCH = CAMERA_PITCH;
const float SPEED = CAMERA_SPEED;
const float SENSITIVITY = CAMERA_SENSITIVITY;
const float ZOOM = CAMERA_ZOOM;
const float MAXZOOM = CAMERA_MAXZOOM;
const float MINZOOM = CAMERA_MINZOOM;

// Camera Class
class Camera
{
public:
	// DATA SETUP

	// VECTOR DATA
	Vec3 Position; // World POS of camera
	Vec3 Front;	   // Forward Direction from camera
	Vec3 Right;	   // Right from Camera
	Vec3 Up;	   // Up from Camera
	Vec3 WorldUp;  // Direction of world Up

	// ANGLE DATA
	float Yaw;	 // angle from top
	float Pitch; // angle from side

	// MOTION DATA
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// Vector constructor
	Camera(Vec3 position = Vec3(0.0f, 0.0f, 0.0f), Vec3 up = Vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	// Component constructor
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
	// Returns view matrix
	Mat4 GetViewMatrix();
	// Processes camera movement from keyboard
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	// Process Camera rotation from mouse
	void ProcessMouseMovement(float xoffset, float yoffset, float deltaTime, GLboolean constrainPitch = true);
	// Processes Camera Zoom from scroll wheel
	void ProcessMouseScroll(float yoffset);

private:
	// Updates front, right and up vectors from angles
	void updateCameraVectors();
};

#endif // !CAMERA_H
