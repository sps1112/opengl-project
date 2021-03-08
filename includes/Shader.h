#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get opengl Files
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <Camera.h>
#include <Light.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

enum SHADER_TYPE
{
	VERTEX_SHADER,
	FRAGMENT_SHADER,
	GEOMETRY_SHADER
};

// Shader Class
class Shader
{
public:
	unsigned int ID;										  // shader program ID
	Shader(const char *vertexPath, const char *fragmentPath); // creates and links shader program
	unsigned int CompileShader(const char *code, SHADER_TYPE type);
	// uses shader program
	void use();
	void FreeData();
	// utility functions
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setVec2(const std::string &name, const glm::vec2 &value) const;
	void setVec2(const std::string &name, float x, float y) const;
	void setVec3(const std::string &name, const glm::vec3 &value) const;
	void setVec3(const std::string &name, float x, float y, float z) const;
	void setVec4(const std::string &name, const glm::vec4 &value) const;
	void setVec4(const std::string &name, float x, float y, float z, float w) const;
	void setMat2(const std::string &name, const glm::mat2 &mat) const;
	void setMat3(const std::string &name, const glm::mat3 &mat) const;
	void setMat4(const std::string &name, const glm::mat4 &mat) const;
	void SetMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);
	void SetMatrices(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
	void SetLight(NormalLight light, float angleVal = 0.0f);
	void SetLights(NormalLight lights[], int count, float angleVal = 0.0f);
	void SetLights(int count, glm::vec3 positions[],
				   glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
				   float angleVal = 0.0f);
	void SetPointLight(glm::vec3 position,
					   glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
					   std::string lightName = "light.",
					   float constant = 1.0f, float linear = 0.22f, float quadratic = 0.2f);
	void SetPointLight(PointLight light, float angleVal = 0.0f);
	void SetPointLights(PointLight lights[], int count, float angleVal = 0.0f);
	void SetDirLight(glm::vec3 direction,
					 glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
					 std::string lightName = "dirLight.");
	void SetDirLight(DirectionalLight light);
	void SetDirLights(DirectionalLight lights[], int count);
	void SetSpotLight(glm::vec3 position, glm::vec3 direction,
					  glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
					  float cutoffAngle, float outerCutoffAngle,
					  std::string lightName = "spotLight.");
	void SetSpotLight(SpotLight light);
	void SetSpotLights(SpotLight lights[], int count);
	void SetScene(glm::vec3 lightColor, float angleVal,
				  glm::vec3 pointLightPositions[], int numberOfLights,
				  glm::vec3 camPos, glm::vec3 camFront);

private:
	// utility function for checking errors
	void CheckCompileErrors(unsigned int shader, std::string type);
};

#endif // !SHADER_H
