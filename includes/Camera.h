#ifndef CAMERA_H
#define CAMERA_H

// glm libraries for maths
#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include <vector>

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
const float SPEED = 2.5f;
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
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// Component constructor
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// returns view matrix
	glm::mat4 GetViewMatrix()
	{
		// Custom lookAt function
		/*
		glm::mat4 translation=glm::mat4(1.0f);
		translation[3][0] = -Position.x;
		translation[3][1] = -Position.y;
		translation[3][2] = -Position.z;
		glm::mat4 rotation = glm::mat4(1.0f);
		rotation[0][0] = Right.x;
		rotation[1][0] = Right.y;
		rotation[2][0] = Right.z;
		rotation[0][1] = Up.x;
		rotation[1][1] = Up.y;
		rotation[2][1] = Up.z;
		rotation[0][2] = -Front.x;
		rotation[1][2] = -Front.y;
		rotation[2][2] = -Front.z;
		return rotation * translation;
		*/
		return glm::lookAt(Position, Position + Front, Up);
	}

	// processes camera movement from keyboard
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
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

	// process Camera rotation from mouse
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

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

	// processes Camera Zoom from scroll wheel
	void ProcessMouseScroll(float yoffset)
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

private:
	// updates front,right and up vectors from angles
	void updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};

#endif // !CAMERA_H
