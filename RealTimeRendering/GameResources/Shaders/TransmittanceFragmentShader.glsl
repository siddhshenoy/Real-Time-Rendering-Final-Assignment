#version 330 core

out vec4 FragColor;

in vec3 fFragPos;
in vec3 fNormal; // this should be normalized
//in vec3 TexCoords;

uniform samplerCube skybox;
uniform vec3 fCameraPosition;

uniform float FresnelPower;
uniform vec3 refractiveIndexRGB;



void main()
{
   vec3 I = normalize(fFragPos - fCameraPosition);
//    vec3 I = normalize(viewPos - Position);

   vec3 nNormal = normalize(fNormal);
   float F = ((1.0 - refractiveIndexRGB.g) * (1.0 - refractiveIndexRGB.g)) / ((1.0 + refractiveIndexRGB.g) * (1.0 + refractiveIndexRGB.g));

   float Ratio = F + (1.0 - F) * pow((1.0 - dot(-I, nNormal)), FresnelPower);

   vec3 RefractR = refract(I, nNormal, refractiveIndexRGB.r);
   vec3 RefractG = refract(I, nNormal, refractiveIndexRGB.g);
   vec3 RefractB = refract(I, nNormal, refractiveIndexRGB.b);

   vec3 Reflect = reflect(I, nNormal);


   vec3 refractColor;
   refractColor.r = texture(skybox, RefractR).r;
   refractColor.g = texture(skybox, RefractG).g;
   refractColor.b = texture(skybox, RefractB).b;

   vec3 reflectColor = texture(skybox, Reflect).rgb;

   vec3 result = mix(reflectColor, refractColor, Ratio);

   FragColor = vec4(result, 1.0);
}












// #version 330 core

// out vec4 FragColor;

// in vec3 fNormal;
// in vec3 fFragPos;

// //struct

// uniform vec3 fCameraPosition;
// uniform float Eta = 0.66f;
// uniform float EtaR = 0.65;
// uniform float EtaG = 0.67; // Ratio of indices of refraction
// uniform float EtaB = 0.69;

// uniform float FresnelPower = 5.0f;

// uniform float fRefractionIndex;
// uniform samplerCube cube;

// void main() {
//     vec3 nNormal = normalize(fNormal);
//     vec3 viewDir = normalize(fCameraPosition - fFragPos);
    
//     float F = ((1.0-EtaG) * (1.0-EtaG)) / ((1.0+EtaG) * (1.0+EtaG));
//     float Ratio = F + (1.0 - F) * pow((1.0 - dot(viewDir, nNormal)), FresnelPower);
//     vec3 reflectedVector = normalize(reflect(-viewDir, nNormal));
//     vec3 refractedVector = normalize(refract(-viewDir, nNormal, Eta));
//     vec3 refR = normalize(refract(-viewDir, nNormal, EtaR));
//     vec3 refG = normalize(refract(-viewDir, nNormal, EtaG));
//     vec3 refB = normalize(refract(-viewDir, nNormal, EtaB));
//     vec3 refractedColor;
//     refractedColor.r = vec3(texture(cube, refR)).r;
//     refractedColor.g = vec3(texture(cube, refG)).g;
//     refractedColor.b = vec3(texture(cube, refB)).b;
//     vec3 reflectedColor = vec3(texture(cube, reflectedVector));
//     vec3 color = mix(reflectedColor, refractedColor, Ratio);
//     // FragColor = mix(texture(cube, reflectedVector), refractedColor, Ratio);
//     FragColor = vec4(color, 1.0);
//     //FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
// }

