#version 330 core
out vec4 FragColor;

uniform vec4 coolColor;

void main() {
    //FragColor = vec4( 1.0f * (gl_FragCoord.x / 800), 1.0f * (gl_FragCoord.y / 600), 1.0f * gl_FragCoord.z, 0.0f );
    FragColor = coolColor;
}