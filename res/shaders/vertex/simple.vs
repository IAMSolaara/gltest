#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 vertexColor;
out vec2 textureCoord;

void main() {
    //vertexColor = vec3(1.0f, 1.0f, 1.0f);
    vertexColor = aColor;
    textureCoord = aTexCoord;
    //gl_Position = vec4(aPos.xyz, 1.0f);
    gl_Position = vec4(aPos, 1.0f);
}