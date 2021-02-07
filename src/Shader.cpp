#include <Shader.h>

Shader::Shader(const char *vertexPath, const char *fragmentPath) // creates and links shader program
{
    // retrieve from file path
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // ensure exceptions
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read buffers into stream
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        //close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream to string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCEESFULLY_READ" << std::endl;
    }

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    unsigned int vertex, fragment;
    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // shader program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete individual shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

// uses shader program
void Shader::use()
{
    glUseProgram(ID);
}

// utility functions
void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string &name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

// Sets the Scene Light Properties
void Shader::SetScene(glm::vec3 lightColor, float angleVal, glm::vec3 pointLightPositions[], int numberOfLights, glm::vec3 camPos, glm::vec3 camFront)
{
    setVec3("dirLight.direction", glm::vec3(0.5f, -1.0f, -0.7f));
    setVec3("dirLight.ambient", lightColor * 0.1f);
    setVec3("dirLight.diffuse", lightColor * 0.25f);
    setVec3("dirLight.specular", glm::vec3(0.5f));

    for (int i = 0; i < numberOfLights; i++)
    {
        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::rotate(lightModel, glm::radians(angleVal), glm::vec3(0.0f, 1.0f, 0.0f));
        lightModel = glm::translate(lightModel, pointLightPositions[i]);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));
        glm::vec3 newLightPos = (lightModel * glm::vec4(1.0f));

        std::string lightName = "pointLights[" + std::to_string(i) + "].";
        setVec3(lightName + "position", newLightPos);
        setFloat(lightName + "constant", 1.0f);
        setFloat(lightName + "linear", 0.22f);
        setFloat(lightName + "quadratic", 0.20f);
        setVec3(lightName + "ambient", lightColor * 0.05f);
        setVec3(lightName + "diffuse", lightColor * 0.4f);
        setVec3(lightName + "specular", glm::vec3(1.0f));
    }
    setVec3("spotLight.position", camPos);
    setVec3("spotLight.direction", camFront);
    setFloat("spotLight.cutoff", glm::cos(glm::radians(12.5f)));
    setFloat("spotLight.outerCutoff", glm::cos(glm::radians(20.0f)));
    setVec3("spotLight.ambient", lightColor * 0.05f);
    setVec3("spotLight.diffuse", lightColor * 0.3f);
    setVec3("spotLight.specular", glm::vec3(0.5f));
}

// utility function for checking errors
void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED of type " << type << "\n"
                      << infoLog << "\n"
                      << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED of type " << type << "\n"
                      << infoLog << "\n"
                      << std::endl;
        }
    }
}