#version 330 core
in vec2 TexCoords;

out vec4 color;

uniform sampler2D textTexture;

void main() {
    color = texture(textTexture, TexCoords);
}