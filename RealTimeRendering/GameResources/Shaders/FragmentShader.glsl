#version 330 core

out vec4 FragColor;

in vec2 fTexCoord;
in vec4 fColor;
in vec3 fNormal;
in vec3 fFragPos;

uniform vec3 fLightColor;
uniform vec3 fViewPos;
uniform vec3 fLightPosition;
uniform vec3 fCameraPosition;

uniform float fSpecularStrength;
uniform float fSpecularPower;

uniform sampler2D Texture;

uniform float fAmbientStrength;
uniform float fDiffuseStrength;

//float AmbientStrength = 0.5;

vec3 FinalColor;

void main()                                                               
{
    //Ambient Calculation
    vec3 AmbientColor = fAmbientStrength * fLightColor; 
    //Diffuse Calculation
    vec3 normal = normalize(fNormal);
    vec3 lightDirection = normalize(fLightPosition - fFragPos);
    float difference = max(dot(normal, lightDirection), 0.0);
    vec3 DiffuseColor = difference * fLightColor * fDiffuseStrength;
    //Specular lighting calculation
    vec3 viewDir = normalize(fCameraPosition - fFragPos);
    vec3 reflectDir = reflect(-lightDirection, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), fSpecularPower);
    vec3 SpecularColor = fSpecularStrength * spec * fLightColor; 

    FinalColor = AmbientColor + DiffuseColor + SpecularColor;

    FragColor = texture(Texture, fTexCoord) * vec4(FinalColor, 1.0) * fColor;
}									 