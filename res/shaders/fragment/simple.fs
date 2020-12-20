#version 330 core
in vec3 vertexColor;

out vec4 FragColor;


void main() {
    //FragColor = vec4( 1.0f * (gl_FragCoord.x / 800), 1.0f * (gl_FragCoord.y / 600), 1.0f * gl_FragCoord.z, 0.0f );
    FragColor = vec4(vertexColor.xyz, 1.0f);
}