//
//  Phong Shading model
//  Link: 
//

#version 330 core

out vec4 FragColor;

in vec2 fTexCoord;
in vec4 fColor;
in vec3 fNormal;
in vec3 fFragPos;

uniform vec3 fObjectColor;

uniform vec3 fLightColor;
uniform vec3 fViewPos;
uniform vec3 fLightPosition;
uniform vec3 fCameraPosition;

uniform float fSpecularStrength;
uniform float fSpecularPower;

uniform sampler2D Texture;
uniform sampler2D NormalMap;

uniform float fAmbientStrength;
uniform float fDiffuseStrength;
uniform int fApplyTexture;
//float AmbientStrength = 0.5;

uniform float fBumpStrength = 2.0f;

vec3 FinalColor;

struct TangentData {
    vec3 LightPos;
    vec3 ViewPos;
    vec3 FragPos;
};
in TangentData fTangentData;

uniform int fApplyMainTexture = 1;
uniform int fApplyNormalTexture = 1;

uniform float fTexMultiplier = 1;

in mat3 TBN;

vec3 calcBumpNormal() {
    vec3 normal = texture(NormalMap, fTexCoord * fTexMultiplier).rgb;
    normal = (2.0 * normal) - vec3(1.0f);
    normal = TBN * normal;
    normal = normalize(normal);
    return normal;
}

void main()
{   

    //Ambient Calculation
    vec3 AmbientColor = fAmbientStrength * fLightColor; 

    vec3 normal = normalize(fNormal);
    //Diffuse Calculation
    if(fApplyNormalTexture == 1)
        normal = calcBumpNormal();
    
    vec3 color = texture(Texture, fTexCoord * fTexMultiplier).rgb;
    if(fApplyMainTexture == 0) {
        color = fObjectColor;
    }

    vec3 lightDirection = normalize(fLightPosition - fFragPos);
    float difference = max(dot(normal, lightDirection), 0.0);
    vec3 DiffuseColor = difference * fLightColor * fDiffuseStrength;

    //Specular lighting calculation
    vec3 viewDir = normalize(fCameraPosition - fFragPos);
    vec3 reflectDir = reflect(-lightDirection, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), fSpecularPower);
    vec3 SpecularColor = fSpecularStrength * spec * fLightColor; 
    FinalColor = AmbientColor * color + DiffuseColor * color + SpecularColor;
    FragColor = vec4(FinalColor, 1.0f);
}									 