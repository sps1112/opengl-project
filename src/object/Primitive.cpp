#include <object/Primitive.h>

Primitive::Primitive()
{
}

Primitive::Primitive(const std::string &path)
{
    filePath = path;
    SetupData((path).c_str());
    SetupPrimitive();
}

void Primitive::SetupTextures(vector<Texture> textures)
{
    this->textures = textures;
}

void Primitive::Draw(Shader &shader)
{
    unsigned int diffusedNR = 1;
    unsigned int specularNR = 1;
    unsigned int normalNR = 1;
    unsigned int heightNR = 1;
    unsigned int emmisionNR = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        SetActiveTexture(i);
        string number;
        string name = textures[i].type;
        if (name == "texture_diffuse")
        {
            number = std::to_string(diffusedNR++);
        }
        else if (name == "texture_specular")
        {
            number = std::to_string(specularNR++);
        }
        else if (name == "texture_normal")
        {
            number = std::to_string(normalNR++);
        }
        else if (name == "texture_height")
        {
            number = std::to_string(heightNR++);
        }
        else if (name == "texture_emmision")
        {
            number = std::to_string(emmisionNR++);
        }
        shader.setInt(("material." + name + number).c_str(), i);
        BindTexture(textures[i].id);
    }
    UnBindTexture();

    // draw mesh
    if (is2D)
    {
        vertexArray.DrawElements(indices.size());
    }
    else
    {
        vertexArray.DrawTriangles(vertexCount, 0);
    }
}

void Primitive::SetupData(const char *path)
{
    std::string pathString;
    std::ifstream file;
    file.open(path);
    std::stringstream statsStream;
    statsStream << file.rdbuf();
    file.close();
    pathString = statsStream.str();
    const char *fileData = pathString.c_str();

    int pathLength = get_char_array_length(path);
    // std::cout << path << std::endl;
    char *extension = get_line(convert_to_char_array(path), pathLength - 4);
    if (compare_with_char_array(extension, convert_to_char_array(".2d")))
    {
        is2D = true;
    }
    else if (compare_with_char_array(extension, convert_to_char_array(".3d")))
    {
        is2D = false;
    }
    vertexCount = GetVertexCount(convert_to_char_array(fileData));
    ProcessData(convert_to_char_array(fileData));
}

int Primitive::GetVertexCount(char *fileData)
{
    int count = 0;
    int lineStartIndex = 0;
    char firstChar = *(fileData + 0);
    char *lineI = get_line(fileData, lineStartIndex);
    while (count == 0 || *(lineI + 0) != firstChar)
    {
        lineStartIndex = get_line_start_index(fileData, lineStartIndex);
        lineI = get_line(fileData, lineStartIndex);
        count++;
    }
    count -= 2;
    // std::cout << "Count is " << count << std::endl;
    return count;
}

void Primitive::ProcessData(char *fileData)
{
    int lineStartIndex = 0;
    char *lineI = get_line(fileData, lineStartIndex);
    char targetChar = ' ';
    char *whitespace = convert_to_char_array(std::string(" ").c_str());
    if (is2D)
    {
        Vertex2D *vertexArray = new Vertex2D[vertexCount];
        for (int i = 0; i < vertexCount; i++)
        {
            lineStartIndex = skip_lines(fileData, lineStartIndex, 1);
            vertexArray[i].Position = GetVec3(fileData, 'v', is2D, lineStartIndex, i);
        }
        // Vertex Set
        lineStartIndex = skip_lines(fileData, lineStartIndex, 2);
        for (int i = 0; i < vertexCount; i++)
        {
            lineStartIndex = skip_lines(fileData, lineStartIndex, 1);
            vertexArray[i].Color = GetVec3(fileData, 'c', is2D, lineStartIndex, i);
        }
        // Color Set
        lineStartIndex = skip_lines(fileData, lineStartIndex, 2);
        for (int i = 0; i < vertexCount; i++)
        {
            lineStartIndex = skip_lines(fileData, lineStartIndex, 1);
            vertexArray[i].TexCoord = GetVec2(fileData, 't', is2D, lineStartIndex, i);
        }
        // Texture Set
        for (int i = 0; i < vertexCount; i++)
        {
            vertices2D.push_back(vertexArray[i]);
        }
        // Setting Indices
        lineStartIndex = skip_lines(fileData, lineStartIndex, 3);
        int i = 0;
        while (((char)fileData[lineStartIndex]) != '\0')
        {
            lineI = get_line(fileData, lineStartIndex);
            // std::cout << lineI << std::endl;

            std::string target = "i " + std::to_string(i) + " ";
            char *val[3];
            int endIndex = get_end_index_string(lineI, convert_to_char_array(target.c_str()));
            for (int i = 0; i < 3; i++)
            {
                int startIndex = get_other_char_index(lineI, endIndex, targetChar);
                lineI = get_line(lineI, startIndex);
                if (i == 2)
                {
                    val[i] = lineI;
                }
                else
                {
                    endIndex = get_end_index_string(lineI, whitespace);
                    val[i] = get_line_part(lineI, 0, endIndex - 1);
                }
            }

            unsigned int a, b, c;
            a = (unsigned int)convert_to_float(val[0]);
            b = (unsigned int)convert_to_float(val[1]);
            c = (unsigned int)convert_to_float(val[2]);
            indices.push_back(a);
            indices.push_back(b);
            indices.push_back(c);
            lineStartIndex = skip_lines(fileData, lineStartIndex, 1);
            i++;
        }
        /*
        // Indices Set
        if (((char)fileData[lineStartIndex]) == '\0')
        {
            std::cout << "End" << std::endl;
        }
        else
        {
            std::cout << "Not end " << ((char)fileData[lineStartIndex]) << "x" << std::endl;
        }
        */
        delete[] vertexArray;
    }
    else
    {
        Vertex3D *vertexArray = new Vertex3D[vertexCount];
        for (int i = 0; i < vertexCount; i++)
        {
            lineStartIndex = skip_lines(fileData, lineStartIndex, 1);
            vertexArray[i].Position = GetVec3(fileData, 'v', is2D, lineStartIndex, i);
        }
        // log_message("Position set");
        lineStartIndex = skip_lines(fileData, lineStartIndex, 2);
        for (int i = 0; i < vertexCount; i++)
        {
            lineStartIndex = skip_lines(fileData, lineStartIndex, 1);
            vertexArray[i].Normal = GetVec3(fileData, 'n', is2D, lineStartIndex, i);
        }
        // log_message("Normal set");
        lineStartIndex = skip_lines(fileData, lineStartIndex, 2);
        for (int i = 0; i < vertexCount; i++)
        {
            lineStartIndex = skip_lines(fileData, lineStartIndex, 1);
            vertexArray[i].TexCoord = GetVec2(fileData, 't', is2D, lineStartIndex, i);
        }
        // log_message("Texture coords set");
        lineStartIndex = skip_lines(fileData, lineStartIndex, 2);
        if (CheckNextLine(fileData, lineStartIndex))
        {
            for (int i = 0; i < vertexCount; i++)
            {
                lineStartIndex = skip_lines(fileData, lineStartIndex, 1);
                vertexArray[i].Color = GetVec3(fileData, 'c', is2D, lineStartIndex, i);
            }
        }
        // log_message("Color set");
        lineStartIndex = skip_lines(fileData, lineStartIndex, 2);
        if (CheckNextLine(fileData, lineStartIndex))
        {
            for (int i = 0; i < vertexCount; i++)
            {
                lineStartIndex = skip_lines(fileData, lineStartIndex, 1);
                vertexArray[i].Tangent = GetVec3(fileData, 'T', is2D, lineStartIndex, i);
            }
            // log_message("Tangent set");
        }
        lineStartIndex = skip_lines(fileData, lineStartIndex, 2);
        if (CheckNextLine(fileData, lineStartIndex))
        {
            for (int i = 0; i < vertexCount; i++)
            {
                lineStartIndex = skip_lines(fileData, lineStartIndex, 1);
                vertexArray[i].Bitangent = GetVec3(fileData, 'b', is2D, lineStartIndex, i);
            }
            // log_message("Bitangent set");
        }
        for (int i = 0; i < vertexCount; i++)
        {
            vertices3D.push_back(vertexArray[i]);
        }
        delete[] vertexArray;
    }
}

void Primitive::SetupPrimitive()
{
    vertexArray.GenerateBuffers();
    vertexArray.BindVAO();
    if (is2D)
    {
        vertexArray.BindVBO(vertices2D.size(), sizeof(Vertex2D), &vertices2D[0]);
        vertexArray.BindEBO(indices.size(), &indices[0]);
    }
    else
    {
        vertexArray.BindVBO(vertices3D.size(), sizeof(Vertex3D), &vertices3D[0]);
    }
    if (is2D)
    {
        // vertex positions
        vertexArray.SetAttribArray(0, 3, sizeof(Vertex2D), (void *)0);
        // vertex color
        vertexArray.SetAttribArray(1, 3, sizeof(Vertex2D), (void *)offsetof(Vertex2D, Color));
        // vertex Texture coordinates
        vertexArray.SetAttribArray(2, 2, sizeof(Vertex2D), (void *)offsetof(Vertex2D, TexCoord));
    }
    else
    {
        // vertex positions
        vertexArray.SetAttribArray(0, 3, sizeof(Vertex3D), (void *)0);
        // vertex normals
        vertexArray.SetAttribArray(1, 3, sizeof(Vertex3D), (void *)offsetof(Vertex3D, Normal));
        // vertex Texture coordinates
        vertexArray.SetAttribArray(2, 2, sizeof(Vertex3D), (void *)offsetof(Vertex3D, TexCoord));
        // vertex colors
        vertexArray.SetAttribArray(3, 3, sizeof(Vertex3D), (void *)offsetof(Vertex3D, Color));
        // vertex Tangents
        vertexArray.SetAttribArray(4, 3, sizeof(Vertex3D), (void *)offsetof(Vertex3D, Tangent));
        // vertex Bitangents
        vertexArray.SetAttribArray(5, 3, sizeof(Vertex3D), (void *)offsetof(Vertex3D, Bitangent));
    }
    // free memory
    vertexArray.UnBindVAO();
}

glm::vec3 GetVec3(char *fileData, char itemType, bool is2D, int lineStartIndex, int vertexIndex)
{
    char targetChar = ' ';
    char *whitespace = convert_to_char_array(std::string(" ").c_str());
    char *lineI = get_line(fileData, lineStartIndex);
    std::string target = std::string(1, itemType) + " ";
    if (!is2D)
    {
        if (vertexIndex < 10)
        {
            target += "0";
        }
    }
    target += std::to_string(vertexIndex) + " ";
    char *val[3];
    int endIndex = get_end_index_string(lineI, convert_to_char_array(target.c_str()));
    for (int i = 0; i < 3; i++)
    {
        int startIndex = get_other_char_index(lineI, endIndex, targetChar);
        lineI = get_line(lineI, startIndex);
        if (i == 2)
        {
            val[i] = lineI;
        }
        else
        {
            endIndex = get_end_index_string(lineI, whitespace);
            val[i] = get_line_part(lineI, 0, endIndex - 1);
        }
    }
    glm::vec3 a;
    a.x = convert_to_float(val[0]);
    a.y = convert_to_float(val[1]);
    a.z = convert_to_float(val[2]);
    return a;
}

glm::vec2 GetVec2(char *fileData, char itemType, bool is2D, int lineStartIndex, int vertexIndex)
{
    char targetChar = ' ';
    char *whitespace = convert_to_char_array(std::string(" ").c_str());
    char *lineI = get_line(fileData, lineStartIndex);
    std::string target = std::string(1, itemType) + " ";
    if (!is2D)
    {
        if (vertexIndex < 10)
        {
            target += "0";
        }
    }
    target += std::to_string(vertexIndex) + " ";
    char *val[2];
    int endIndex = get_end_index_string(lineI, convert_to_char_array(target.c_str()));
    for (int i = 0; i < 2; i++)
    {
        int startIndex = get_other_char_index(lineI, endIndex, targetChar);
        lineI = get_line(lineI, startIndex);
        if (i == 1)
        {
            val[i] = lineI;
        }
        else
        {
            endIndex = get_end_index_string(lineI, whitespace);
            val[i] = get_line_part(lineI, 0, endIndex - 1);
        }
    }
    glm::vec2 a;
    a.x = convert_to_float(val[0]);
    a.y = convert_to_float(val[1]);
    return a;
}

bool CheckNextLine(char *fileData, int lineStartIndex)
{
    char testChar = ((char)fileData[skip_lines(fileData, lineStartIndex, 1)]);
    bool status = (testChar != ' ' && testChar != '\0' && testChar != '\n');
    return status;
}