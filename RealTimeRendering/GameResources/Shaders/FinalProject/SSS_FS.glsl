#version 330 core



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
uniform sampler2D gBeckmannTexture;
uniform sampler2D gSpecularMap;
uniform sampler2D NormalMap;

uniform float fAmbientStrength;
uniform float fDiffuseStrength;
uniform int fApplyTexture;

uniform float fFresnelFactor = 1.0f;
uniform float cosThetaPower;

uniform float fTexMultiplier = 1;

uniform int useTexture = 1;
uniform int useBeckmannTexture = 1;
uniform int useNormalMap = 1;
uniform int useOnlySpecular = 0;
//float AmbientStrength = 0.5;

uniform float scale = 200.0; // Scale parameter for the scattering effect
uniform float bias = 100.0; // Bias parameter for the scattering effect
out vec4 outColor;

in mat3 TBN;

vec2 FLIPPED_UV = vec2(fTexCoord.x, 1.0f - fTexCoord.y);

vec3 calcBumpNormal() {
    vec3 normal = texture(NormalMap, fTexCoord).rgb;
    normal = (2.0 * normal) - vec3(1.0f);
    normal = TBN * normal;
    normal = normalize(normal);
    return normal;
}

// float fresnelReflectance( vec3 H, vec3 V, float F0 )
// {
//     float base = 1.0 - dot( V, H );
//     float exponential = pow( base, 5.0 );
//     return exponential + F0 * ( 1.0 - exponential );
// }

float fresnelReflectance(float cosTheta, float F0)
{
    cosTheta = 1.0f - cosTheta;
    cosTheta = pow(cosTheta, cosThetaPower);
    return cosTheta + (1.0f - cosTheta) * F0;
}
 
float KS_Skin_Specular(vec3 N, vec3 L, vec3 V, float m, float rho_s)
{
    float result = 0.0f;
    float NoL = dot(N, L);
 
    if (NoL > 0.0f)
    {
        vec3 h = L + V;
        vec3 H = normalize(h);
        float NoH = dot(N, H);
        float PH = pow(2.0f * texture(gBeckmannTexture, vec2(NoH, m)).r, 10);
        float cosTheta = dot(H, V);
        float F = fresnelReflectance(cosTheta, fFresnelFactor);
        if(useBeckmannTexture == 0) return F;
        float frSpec = max(PH * F / dot(h, h), 0);
        result = NoL * rho_s * frSpec; // BRDF * dot(N,L) * rho_s
    }
    return result;
}

vec3 DirectionalDipole(vec3 pos, vec3 normal, vec3 lightDir, float scale, float bias) {
    // Compute the distance between the surface point and the light source
    float dist = length(pos - lightDir);

    // Compute the scattering radius using the scale parameter
    float r = scale * dist;

    // Compute the attenuation factor based on the distance and bias parameter
    float a = exp(-bias * dist);

    // Compute the direction of the scattered light
    vec3 s = lightDir - 2.0 * dot(lightDir, normal) * normal;

    // Compute the amount of light scattered in the direction s
    vec3 c = (1.0 / (4.0 * 3.14159265 * r * r)) * exp(-dist / r) * vec3(a);

    return c;
}

void main() {
    vec3 pos = gl_FragCoord.xyz;
    vec3 normal = normalize(fNormal);
    vec3 color = texture(Texture,fTexCoord).rgb; // Replace with your own base color
    vec3 lightDirection = normalize(fLightPosition - fFragPos);

    

    // Compute the subsurface scattering effect using the directional dipole model
    vec3 scattering = DirectionalDipole(pos, normal, lightDirection, scale, bias);


    vec3 AmbientColor = fAmbientStrength * fLightColor; 

    //Diffuse Calculation
    
    if(useNormalMap == 1)
        normal = calcBumpNormal();
    
    float difference = max(dot(normal, lightDirection), 0.0);
    vec3 DiffuseColor = difference * fLightColor * fDiffuseStrength;

    //Specular lighting calculation
    vec3 viewDir = normalize(fCameraPosition - fFragPos);
    vec3 reflectDir = reflect(-lightDirection, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), fSpecularPower);

    vec3 halfAngle = normalize(lightDirection + viewDir);
    vec3 SpecularColor = fSpecularStrength * spec * fLightColor;
    //FinalColor = (SpecularColor * KS_Skin_Specular(normal, lightDirection, viewDir, m, rho_s));
    vec4 specTap = texture(gSpecularMap, FLIPPED_UV); // rho_s and m (roughness)
    float m = specTap.w * 0.09f + 0.23f;
    float rho_s = specTap.x * 0.16f + 0.18f;
    rho_s *= float(specTap.x > 0.1f);
    vec3 FinalColor;
    vec4 FragColor;
    if(useOnlySpecular == 1)
        FinalColor = (SpecularColor * KS_Skin_Specular(normal, lightDirection, viewDir, m, rho_s));
    else
        FinalColor = AmbientColor + DiffuseColor + (SpecularColor * KS_Skin_Specular(normal, lightDirection, viewDir, m, rho_s));
    //FinalColor = AmbientColor + DiffuseColor + (SpecularColor * fresnelReflectance(halfAngle, viewDir, fFresnelFactor));
    //(SpecularColor * fresnelReflectance(halfAngle, viewDir, fFresnelFactor));
    if(useTexture == 1)
        FragColor = texture(Texture, fTexCoord) * vec4(FinalColor, 1.0); //* fColor * vec4(fObjectColor, 1.0f);
    else
        FragColor = vec4(FinalColor, 1.0); //* fColor * vec4(fObjectColor, 1.0f);

    // Add the subsurface scattering effect to the base color
    outColor = vec4(FragColor.rgb, 1.0);
}