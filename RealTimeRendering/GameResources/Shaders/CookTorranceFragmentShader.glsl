//
//	Cook Torrance FragmentShader
//	Link: https://github.com/pboechat/cook_torrance/blob/master/application/shaders/cook_torrance_textured.fs.glsl
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

uniform float F0 = 0.8;
uniform float roughness = 0.1;
uniform float k = 0.2;

uniform int fApplyTexture;
const float PI = 3.14159f;

vec3 FinalColor;

vec3 CookTorrance(vec3 materialDiffuseColor, vec3 materialSpecularColor, vec3 normal, vec3 lightDir, vec3 viewDir, vec3 lightColor)
{
	//vec3 AmbientColor = fAmbientStrength * lightColor;
	float NdotL = max(0, dot(normal, lightDir));
	float Rs = 0.0f;
	if (NdotL > 0.0f) 
	{
		vec3 H = normalize(lightDir + viewDir);
		float NdotH = max(0, dot(normal, H));
		float NdotV = max(0, dot(normal, viewDir));
		float VdotH = max(0, dot(lightDir, H));

		// Fresnel reflectance
		float F = pow(1.0 - VdotH, 5.0);
		F *= (1.0 - F0);
		F += F0;

		// Microfacet distribution by Beckmann
		float m_squared = roughness * roughness;
		float r1 = 1.0 / (4.0 * m_squared * pow(NdotH, 4.0));
		float r2 = (NdotH * NdotH - 1.0) / (m_squared * NdotH * NdotH);
		float D = r1 * exp(r2);

		// Geometric shadowing
		float two_NdotH = 2.0 * NdotH;
		float g1 = (two_NdotH * NdotV) / VdotH;
		float g2 = (two_NdotH * NdotL) / VdotH;
		float G = min(1.0, min(g1, g2));

		Rs = (F * D * G) / (4 * NdotL * NdotV);
        
	}
    return (materialDiffuseColor * lightColor * NdotL * fDiffuseStrength) + (materialSpecularColor * lightColor * NdotL * (k + Rs * (1.0f - k)) * fSpecularStrength);
	
}

void main()
{
	if(fApplyTexture == 1)
    	FragColor = vec4(CookTorrance(texture(Texture, fTexCoord).rgb, vec3(1.0f, 1.0f, 1.0f), normalize(fNormal), normalize(fLightPosition - fFragPos), normalize(fCameraPosition - fFragPos), fLightColor), 1.0);
	else
		FragColor = vec4(CookTorrance(vec3(1.0f), vec3(1.0f, 1.0f, 1.0f), normalize(fNormal), normalize(fLightPosition - fFragPos), normalize(fCameraPosition - fFragPos), fLightColor), 1.0);
}