//
//	Oren-Nayar FragmentShader
//	References taken from the link: https://garykeen27.wixsite.com/portfolio/oren-nayar-shading
//
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

uniform float fRoughness;
uniform float fIrradiance;
uniform float fAlbedo;

uniform int fApplyTexture;
const float PI = 3.14159f;

vec3 FinalColor;

void main()
{   
    //Ambient Calculation
    vec3 AmbientColor = fAmbientStrength * fLightColor; 

    //Diffuse Calculation
    vec3 normal = normalize(fNormal);
    vec3 lightDirection = normalize(fLightPosition - fFragPos);
    float difference = max(dot(normal, lightDirection), 0.0);
    vec3 viewDir = normalize(fCameraPosition - fFragPos);
    float NdotV = clamp(dot(normal, viewDir), 0.0f, 1.0f);
    float NdotL = clamp(dot(normal, lightDirection), 0.0f, 1.0f);
    float angleVN = acos(NdotV);
    float angleLN = acos(NdotL);

    float alpha = max(angleVN, angleLN);
    float beta = min(angleVN, angleLN);
    float gamma = cos(angleVN - angleLN);

    float sqr_rough = pow(fRoughness, 2.0);

    float A = 1.0 - (0.5 * (sqr_rough / (sqr_rough + 0.33)));
    float B = 0.45 * (sqr_rough / ( sqr_rough + 0.09 ));
    float C = sin(alpha) * tan(beta);


    float Lr = (fAlbedo / PI) *  fIrradiance * NdotL * (A + (B * max(0.0f, gamma) * C));


    vec3 DiffuseColor = fLightColor * fDiffuseStrength * Lr;


    // //Specular lighting calculation
    
    // vec3 reflectDir = reflect(-lightDirection, normal);  
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), fSpecularPower);
    // vec3 SpecularColor = fSpecularStrength * spec * fLightColor; 



    FinalColor = AmbientColor + DiffuseColor;
    if(fApplyTexture == 1)
        FragColor = texture(Texture, fTexCoord) * vec4(FinalColor, 1.0) * fColor;
    else
        FragColor = vec4(FinalColor, 1.0) * fColor;
}									 