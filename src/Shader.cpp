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
    vertex = CompileShader(vShaderCode, VERTEX_SHADER);
    CheckCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = CompileShader(fShaderCode, FRAGMENT_SHADER);
    CheckCompileErrors(fragment, "FRAGMENT");

    // shader program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    CheckCompileErrors(ID, "PROGRAM");
    // delete individual shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

unsigned int Shader::CompileShader(const char *code, SHADER_TYPE type)
{
    unsigned int shader;
    if (type == VERTEX_SHADER)
    {
        shader = glCreateShader(GL_VERTEX_SHADER);
    }
    else if (type == FRAGMENT_SHADER)
    {
        shader = glCreateShader(GL_FRAGMENT_SHADER);
    }
    else if (type == GEOMETRY_SHADER)
    {
        shader = glCreateShader(GL_GEOMETRY_SHADER);
    }
    glShaderSource(shader, 1, &code, NULL);
    glCompileShader(shader);
    return shader;
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

void Shader::SetMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
{
    setVec3("material.ambient", ambient);
    setVec3("material.diffuse", diffuse);
    setVec3("material.specular", specular);
    setFloat("material.shininess", 64);
}

void Shader::SetMatrices(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
    setMat4("model", model);
    setMat4("view", view);
    setMat4("projection", projection);
}

void Shader::SetPointLight(glm::vec3 position,
                           glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                           float constant, float linear, float quadratic,
                           bool hasName, std::string lightName)
{
    if (hasName)
    {
        setVec3(lightName + "position", position);
        setFloat(lightName + "constant", constant);
        setFloat(lightName + "linear", linear);
        setFloat(lightName + "quadratic", quadratic);
        setVec3(lightName + "ambient", ambient);
        setVec3(lightName + "diffuse", diffuse);
        setVec3(lightName + "specular", specular);
    }
    else
    {
        setVec3("pointLight.position", position);
        setFloat("pointLight.constant", constant);
        setFloat("pointLight.linear", linear);
        setFloat("pointLight.quadratic", quadratic);
        setVec3("pointLight.ambient", ambient);
        setVec3("pointLight.diffuse", diffuse);
        setVec3("pointLight.specular", specular);
    }
}
void Shader::SetDirLight(glm::vec3 direction,
                         glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
    setVec3("dirLight.direction", direction);
    setVec3("dirLight.ambient", ambient);
    setVec3("dirLight.diffuse", diffuse);
    setVec3("dirLight.specular", specular);
}
void Shader::SetSpotLight(glm::vec3 position, glm::vec3 direction,
                          glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                          float cutoffAngle, float outerCutoffAngle)
{
    setVec3("spotLight.position", position);
    setVec3("spotLight.direction", direction);
    setFloat("spotLight.cutoff", glm::cos(glm::radians(cutoffAngle)));
    setFloat("spotLight.outerCutoff", glm::cos(glm::radians(outerCutoffAngle)));
    setVec3("spotLight.ambient", ambient);
    setVec3("spotLight.diffuse", diffuse);
    setVec3("spotLight.specular", specular);
}

// Sets the Scene Light Properties
void Shader::SetScene(glm::vec3 lightColor, float angleVal, glm::vec3 pointLightPositions[], int numberOfLights, glm::vec3 camPos, glm::vec3 camFront)
{
    SetDirLight(glm::vec3(0.5f, -1.0f, -0.7f), lightColor * 0.1f, lightColor * 0.3f, lightColor * 0.5f);

    for (int i = 0; i < numberOfLights; i++)
    {
        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::rotate(lightModel, glm::radians(angleVal), glm::vec3(0.0f, 1.0f, 0.0f));
        lightModel = glm::translate(lightModel, pointLightPositions[i]);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));
        glm::vec3 newLightPos = (lightModel * glm::vec4(1.0f));

        std::string lightName = "pointLights[" + std::to_string(i) + "].";
        SetPointLight(newLightPos,
                      lightColor * 0.05f, lightColor * 0.4f, lightColor,
                      1.0f, 0.22f, 0.20f, true, lightName);
    }
    SetSpotLight(camPos, camFront, lightColor * 0.05f, lightColor * 0.30f, lightColor * 0.5f, 12.5f, 20.0f);
}

// utility function for checking errors
void Shader::CheckCompileErrors(unsigned int shader, std::string type)
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