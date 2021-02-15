#version 460 core
struct Material {
  sampler2D texture_diffuse1;
};
uniform Material material;
uniform int filterChoice;
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform int width;
uniform int height;
uniform int boxSize;

const float offset = 1.0 / 300.0;

void main() {
  vec3 finalColor = vec3(texture(material.texture_diffuse1, TexCoord));
  if (filterChoice == 1) // Invert Colors
  {
    finalColor =
        vec3(1.0f) - vec3(texture(material.texture_diffuse1, TexCoord));
  } else if (filterChoice == 2) // Weighted Grayscale
  {
    float average =
        0.2126 * finalColor.x + 0.7152 * finalColor.y + 0.0722 * finalColor.z;
    finalColor = vec3(average);
  } else if (filterChoice >= 3 && filterChoice <= 7) // Kernel Filters
  {
    vec2 offsets[9] = vec2[](vec2(-offset, offset),  // top-left
                             vec2(0.0f, offset),     // top-center
                             vec2(offset, offset),   // top-right
                             vec2(-offset, 0.0f),    // center-left
                             vec2(0.0f, 0.0f),       // center-center
                             vec2(offset, 0.0f),     // center-right
                             vec2(-offset, -offset), // bottom-left
                             vec2(0.0f, -offset),    // bottom-center
                             vec2(offset, -offset)   // bottom-right
    );
    float kernel[9] = float[](0, 0, 0, 0, 1, 0, 0, 0, 0); // Identity Kernel
    if (filterChoice == 3)                                // Sharpen Kernel
    {
      kernel[0] = -1;
      kernel[1] = -1;
      kernel[2] = -1;
      kernel[3] = -1;
      kernel[4] = 9;
      kernel[5] = -1;
      kernel[6] = -1;
      kernel[7] = -1;
      kernel[8] = -1;
    } else if (filterChoice == 4) // Blur Kernel
    {
      kernel[0] = 1.0 / 16;
      kernel[1] = 2.0 / 16;
      kernel[2] = 1.0 / 16;
      kernel[3] = 2.0 / 16;
      kernel[4] = 4.0 / 16;
      kernel[5] = 2.0 / 16;
      kernel[6] = 1.0 / 16;
      kernel[7] = 2.0 / 16;
      kernel[8] = 1.0 / 16;
    } else if (filterChoice == 5) // Edge Detection
    {
      kernel[0] = 1;
      kernel[1] = 1;
      kernel[2] = 1;
      kernel[3] = 1;
      kernel[4] = -8;
      kernel[5] = 1;
      kernel[6] = 1;
      kernel[7] = 1;
      kernel[8] = 1;
    } else if (filterChoice == 6) // Emboss Kernel
    {
      kernel[0] = -2;
      kernel[1] = -1;
      kernel[2] = 0;
      kernel[3] = -1;
      kernel[4] = 1;
      kernel[5] = 1;
      kernel[6] = 0;
      kernel[7] = 1;
      kernel[8] = 2;
    }
    if (filterChoice == 7) // Outline Kernel
    {
      kernel[0] = -1;
      kernel[1] = -1;
      kernel[2] = -1;
      kernel[3] = -1;
      kernel[4] = 8;
      kernel[5] = -1;
      kernel[6] = -1;
      kernel[7] = -1;
      kernel[8] = -1;
    }

    vec3 sampleTex[9];
    for (int i = 0; i < 9; i++) {
      sampleTex[i] =
          vec3(texture(material.texture_diffuse1, TexCoord.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for (int i = 0; i < 9; i++) {
      col += sampleTex[i] * kernel[i];
    }
    finalColor = col;
  } else if (filterChoice == 8) // Checkbox
  {
    int posX = int(gl_FragCoord.x / boxSize);
    int posY = int(gl_FragCoord.y / boxSize);
    float average =
        0.2126 * finalColor.x + 0.7152 * finalColor.y + 0.0722 * finalColor.z;
    if ((posX % 2) == (posY % 2)) // both odd or even
    {
      finalColor = vec3(average);
    } else {
      finalColor = vec3(average * 3);
    }
  }
  FragColor = vec4(finalColor, 1.0);
}