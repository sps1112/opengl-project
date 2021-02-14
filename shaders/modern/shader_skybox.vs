#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aColor;
layout(location = 4) in vec3 aTangent;
layout(location = 5) in vec3 aBitangent;

uniform mat4 view;
uniform mat4 projection;

out vec3 TexPos;

void main() {
  TexPos = aPos;
  vec4 pos = (projection * view * vec4(aPos, 1.0f));
  gl_Position = pos.xyww;
}