#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec4 vColor;
layout (location = 2) in vec3 vNormal;
layout (location = 3) in vec2 vTexCoord;

uniform mat4 transform;
uniform mat4 model;

out vec2 fTexCoord;
out vec4 fColor;
out vec3 fNormal;
out vec3 fFragPos;

void main()
{
    fNormal = mat3(transpose(inverse(model))) * vNormal;
    fFragPos = vec3(model * vec4(vPosition, 1.0)); 
    fTexCoord = vTexCoord;
    fColor = vColor;
    gl_Position = transform * vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
}