#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec4 vColor;
layout (location = 2) in vec3 vNormal;
layout (location = 3) in vec2 vTexCoord;
layout (location = 4) in vec3 vTangent;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
}