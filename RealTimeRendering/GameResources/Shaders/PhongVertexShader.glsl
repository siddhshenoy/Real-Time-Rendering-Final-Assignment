#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec4 vColor;
layout (location = 2) in vec3 vNormal;
layout (location = 3) in vec2 vTexCoord;
layout (location = 4) in vec3 vTangent;

uniform mat4 transform;
uniform mat4 model;

out vec2 fTexCoord;
out vec4 fColor;
out vec3 fNormal;
out vec3 fFragPos;

struct TangentData {
    vec3 LightPos;
    vec3 ViewPos;
    vec3 FragPos;
};
out TangentData fTangentData;

out mat3 TBN;

uniform vec3 fLightPosition;
uniform vec3 fCameraPosition;


uniform int fFlipTangentCalculation = 0;
uniform int fFlipBitangentCross = 0;
uniform int fFlipNormalInMatrix = 0;
uniform int fFlipTangent = 0;
void main()
{
    mat3 normalMatrix = mat3(transpose(inverse(model)));
    fNormal = mat3(transpose(inverse(model))) * vNormal;

    fFragPos = vec3(model * vec4(vPosition, 1.0)); 
    fTexCoord = vTexCoord;
    fColor = vColor;

    //Calculate the Tangent, Bi-Tangent and Normal matrix
    vec3 T = normalize(normalMatrix * vTangent);
    vec3 N = normalize(normalMatrix * vNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(T, N);
    if(fFlipBitangentCross == 1)
        B = cross(N, T);
    TBN = mat3(T, B, N);
    gl_Position = transform * vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
}