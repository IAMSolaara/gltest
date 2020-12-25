#version 330 core
in vec3 vertexColor;
in vec2 textureCoord;

out vec4 FragColor;

//uniform sampler2D mytexture;

void main() {
    //FragColor = vec4( 1.0f * (gl_FragCoord.x / 800), 1.0f * (gl_FragCoord.y / 600), 1.0f * gl_FragCoord.z, 0.0f );
    //FragColor = vec4(vertexColor.xyz, 1.0f);
    FragColor = vec4(vertexColor, 1.0f);
}
