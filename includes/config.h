#ifndef CONFIG_H
#define CONFIG_H

// All the Configuration details for the Project

// OpenGL Settings
#define OPENGL_MAJOR_VERSION 4
#define OPENGL_MINOR_VERSION 6

// Window Settings
#define DEFAULT_WINDOW_HEIGHT 600
#define DEFAULT_WINDOW_WIDTH 800

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
#define CAMERA_YAW -90.0f
#define CAMERA_PITCH 0.0f
#define CAMERA_SPEED 4.5f
#define CAMERA_SENSITIVITY 0.1f
#define CAMERA_ZOOM 45.0f
#define CAMERA_MAXZOOM 60.0f
#define CAMERA_MINZOOM 1.0f

// Scene Settings
#define MAX_LOADED_SCENES_COUNT 3

// Renderer Settings
#define GAMMA_CORRECTION_ENABLED 1
#define MAX_POINT_LIGHTS 6
#define MAX_DIRECTIONAL_LIGHTS 4
#define MAX_SPOT_LIGHT 4
#define MAX_LIGHTS 12

#endif // !CONFIG_H