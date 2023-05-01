#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D sprite_texture;
uniform vec4 sprite_color;

void main()
{
    color = texture(sprite_texture, TexCoords) * sprite_color; // + vec4(1.0, 1.0, 1.0, 0.0);// * vec4(sprite_color, 1.0);
}