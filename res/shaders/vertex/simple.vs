#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;

void main() {
    //vertexColor = vec3(1.0f, 1.0f, 1.0f);
    vertexColor = aColor;
    //gl_Position = vec4(aPos.xyz, 1.0f);
    gl_Position = vec4(aPos, 1.0f);
}