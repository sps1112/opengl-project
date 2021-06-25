#ifndef CAMERA_H
#define CAMERA_H

// Header declarations
#include <external/glad/glad.h>
#include <utility/CustomMath.h>
#include <config.h>

#include <vector>
#include <iostream>

// Camera movement directions
enum CAMERA_MOVE_DIR
{
	FORWARD_DIR,
	BACKWARD_DIR,
	LEFT_DIR,
	RIGHT_DIR,
	UP_DIR,
	DOWN_DIR
};

// Camera Class
class Camera
{
public:
	// DATA SETUP

	// VECTOR DATA
	Vec3 position; // World POS of camera
	Vec3 front;	   // Forward Direction from camera
	Vec3 right;	   // Right from Camera
	Vec3 up;	   // Up from Camera
	Vec3 worldUp;  // Direction of world Up

	// ANGLE DATA
	float yaw;	 // angle from top
	float pitch; // angle from side

	// MOTION DATA
	float moveSpeed;		// Movement speed of the Camera
	float mouseSensitivity; // Mouse sensitivity of the Camera
	float fovZoom;			// Field of View of the Camera

	// Vector constructor
	Camera(Vec3 position_ = Vec3(0.0f, 0.0f, 0.0f), Vec3 up = Vec3(0.0f, 1.0f, 0.0f), float yaw_ = CAMERA_YAW, float pitch_ = CAMERA_PITCH);
	// Component constructor
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw_, float pitch_);
	// Returns view matrix
	Mat4 get_view_matrix();
	// Processes camera movement from keyboard
	void process_keyboard(CAMERA_MOVE_DIR direction, float deltaTime);
	// Process Camera rotation from mouse
	void process_mouse(float xoffset, float yoffset, float deltaTime, GLboolean constrainPitch = true);
	// Processes Camera Zoom from scroll wheel
	void process_scroll(float yoffset);
	// Updates front, right and up vectors from angles
	void update_camera_vectors();

private:
};

#endif // !CAMERA_H
