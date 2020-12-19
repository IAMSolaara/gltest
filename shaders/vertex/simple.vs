#version 330 core
layout (location = 0) in vec3 aPos;
void main() {
    //gl_Position = vec4(aPos.xyz, 1.0f);
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
}