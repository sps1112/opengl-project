#ifndef CONFIG_H
#define CONFIG_H

// All the Configuration Settings for the Project

// OpenGL Settings

#define OPENGL_MAJOR_VERSION 4
#define OPENGL_MINOR_VERSION 6
#define OPENGL_PROFILE_USED GLFW_OPENGL_CORE_PROFILE

// Window Settings

#define DEFAULT_WINDOW_HEIGHT 600
#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_TITLE "OpenGL Window"
#define ENABLE_FULLSCREEN_MODE 0
#define ASPECT_RATIO_LOCKED 1
#define ASPECT_RATIO Vec2(4, 3)
#define WINDOW_SIZE_LIMITED 0
#define WINDOW_SIZE_LIMITS Vec4(160, 120, 1000, 750)

// Vector Settings

#define WORLD_UP Vec3(0.0f, 1.0f, 0.0f)
#define WORLD_RIGHT Vec3(1.0f, 0.0f, 0.0f)
#define WORLD_FORWARD Vec3(0.0f, 0.0f, 1.0f)

// Color Settings

#define COLOR_BLACK Vec3(0.0f, 0.0f, 0.0f)
#define COLOR_GRAY Vec3(0.1f, 0.1f, 0.1f)
#define COLOR_RED Vec3(1.0f, 0.1f, 0.1f)
#define COLOR_GREEN Vec3(0.1f, 1.0f, 0.1f)
#define COLOR_BLUE Vec3(0.1f, 0.1f, 1.0f)
#define COLOR_YELLOW Vec3(1.0f, 1.0f, 0.1f)
#define COLOR_WHITE Vec3(1.0f, 1.0f, 1.0f)
#define COLOR_PURPLE Vec3(1.0f, 0.1f, 1.0f)
#define DEFAULT_BACKGROUND_COLOR COLOR_BLACK
#define DEFAULT_SCENE_COLOR COLOR_GRAY
#define DEFAULT_SHADER_COLOR COLOR_PURPLE

// Camera Settings

#define CAMERA_ORIGIN Vec3(0.0f, 0.0f, 5.0f)
#define CAMERA_YAW -90.0f
#define CAMERA_PITCH 0.0f
#define CAMERA_MAX_PITCH 89.0f
#define CAMERA_SPEED 4.5f
#define CAMERA_SENSITIVITY 0.1f
#define CAMERA_ZOOM 45.0f
#define CAMERA_MAXZOOM 60.0f
#define CAMERA_MINZOOM 1.0f
#define CAMERA_FAR_PLANE 100.0f
#define CAMERA_NEAR_PLANE 0.01f

// Scene Settings

#define MAX_LOADED_SCENES_COUNT 3

// Renderer Settings

#define BLINN_PHONG_ENABLE 1
#define GAMMA_CORRECTION_ENABLED 1
#define MAX_POINT_LIGHTS 6
#define MAX_DIRECTIONAL_LIGHTS 4
#define MAX_SPOT_LIGHT 4
#define MAX_LIGHTS 12

// Key Settings

#define KEY_ESC GLFW_KEY_ESCAPE
#define KEY_W GLFW_KEY_W
#define KEY_S GLFW_KEY_S
#define KEY_A GLFW_KEY_A
#define KEY_D GLFW_KEY_D
#define KEY_Q GLFW_KEY_Q
#define KEY_E GLFW_KEY_E
#define KEY_SPACE GLFW_KEY_SPACE
#define KEY_LFT_ALT GLFW_KEY_LEFT_ALT

#endif // !CONFIG_H
