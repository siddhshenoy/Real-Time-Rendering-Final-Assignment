#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec4 vColor;
layout (location = 2) in vec3 vNormal;
layout (location = 3) in vec2 vTexCoord;

uniform mat4 transform;
out vec4 fColor;

void main()
{
    fColor = vColor;
    gl_Position = transform * vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
}