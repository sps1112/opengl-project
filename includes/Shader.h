#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get opengl Files
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <Camera.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// Shader Class
class Shader
{
public:
	unsigned int ID;										  // shader program ID
	Shader(const char *vertexPath, const char *fragmentPath); // creates and links shader program
	// uses shader program
	void use();
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
	void SetPointLight(glm::vec3 position,
					   glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
					   float constant, float linear, float quadratic, bool hasName = false, std::string lightName = "");
	void SetDirLight(glm::vec3 direction,
					 glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	void SetSpotLight(glm::vec3 position, glm::vec3 direction,
					  glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
					  float cutoffAngle, float outerCutoffAngle);
	void SetScene(glm::vec3 lightColor, float angleVal,
				  glm::vec3 pointLightPositions[], int numberOfLights,
				  glm::vec3 camPos, glm::vec3 camFront);

private:
	// utility function for checking errors
	void checkCompileErrors(unsigned int shader, std::string type);
};

#endif // !SHADER_H
