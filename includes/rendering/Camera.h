#ifndef CAMERA_H
#define CAMERA_H

// glm libraries for maths
#include <external/glad/glad.h>
#include <external/glm/glm/glm.hpp>
#include <external/glm/glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>

// camera movement directions
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// default values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 4.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
const float MAXZOOM = 60.0f;
const float MINZOOM = 1.0f;

// Camer Class
class Camera
{
public:
	// DATA SETUP

	// VECTOR DATA
	glm::vec3 Position; // World POS of camera
	glm::vec3 Front;	// Forward Direction from camera
	glm::vec3 Right;	// Right from Camera
	glm::vec3 Up;		// Up from Camera
	glm::vec3 WorldUp;	// Direction of world Up

	// ANGLE DATA
	float Yaw;	 // angle from top
	float Pitch; // angle from side

	// MOTION DATA
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// Vector constructor
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	// Component constructor
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
	// returns view matrix
	glm::mat4 GetViewMatrix();
	// processes camera movement from keyboard
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	// process Camera rotation from mouse
	void ProcessMouseMovement(float xoffset, float yoffset, float deltaTime, GLboolean constrainPitch = true);
	// processes Camera Zoom from scroll wheel
	void ProcessMouseScroll(float yoffset);

private:
	// updates front,right and up vectors from angles
	void updateCameraVectors();
};

#endif // !CAMERA_H
