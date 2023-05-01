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
uniform int fIntensityMultiplier;
uniform int fApplyTexture;
//float AmbientStrength = 0.5;

vec3 FinalColor;



void main()                                                               
{
/*	
	float intensity;
	vec4 color;
	vec3 normal = normalize(fNormal);
	intensity = dot(vec3(fLightPosition), normal);

	if (intensity > 0.95)
		color = vec4(1.0,0.5,0.5,1.0);
	else if (intensity > 0.5)
		color = vec4(0.6,0.3,0.3,1.0);
	else if (intensity > 0.25)
		color = vec4(0.4,0.2,0.2,1.0);
	else
		color = vec4(0.2,0.1,0.1,1.0);
	FragColor = color;
*/
	vec3 normal = normalize(fNormal);
	vec3 direction = normalize(fFragPos - fLightPosition);
	vec3 eye = normalize(-fFragPos);
	vec3 reflect_direction = normalize(reflect(direction, normal));
	float spec = max(dot(reflect_direction, eye), 0.0f);
	float diffuse = max(dot(-direction, normal), 0.0f);
	//float intensity = (fIntensityMultiplier * diffuse) + ((1 - fIntensityMultiplier) * spec);
	float intensity = diffuse;
	vec4 color;
	if (fIntensityMultiplier == 1) {
		if (intensity > 0.95)
			color = vec4(1.0,1.0,1.0,1.0);
		else if (intensity > 0.5)
			color = vec4(0.6,0.6,0.6,1.0);
		else if (intensity > 0.25)
			color = vec4(0.3,0.3,0.3,1.0);
		else
			color = vec4(0.1,0.1,0.1,1.0);
	} else if (fIntensityMultiplier == 2) {
		if (intensity > 0.95)
			color = vec4(1.0,1.0,1.0,1.0);
		else if (intensity > 0.7)
			color = vec4(0.75,0.75,0.75,1.0);
		else if (intensity > 0.5)
			color = vec4(0.5,0.5,0.5,1.0);
		else if (intensity > 0.25)
			color = vec4(0.25,0.25,0.25,1.0);
		else
			color = vec4(0.1,0.1,0.1,1.0);
	} else if (fIntensityMultiplier == 3) {
		if (intensity > 0.95)
			color = vec4(1.0,1.0,1.0,1.0);
		else if (intensity > 0.8)
			color = vec4(0.8, 0.8, 0.8, 1.0);
		else if (intensity > 0.5)
			color = vec4(0.5,0.5,0.5,1.0);
		else if (intensity > 0.35)
			color = vec4(0.35,0.35,0.35,1.0);
		else if (intensity > 0.15)
			color = vec4(0.15,0.15,0.15,1.0);
		else
			color = vec4(0.1,0.1,0.1,1.0);
	} else if (fIntensityMultiplier > 3) {
		if (intensity > 0.95)
			color = vec4(1.0,1.0,1.0,1.0);
		else if (intensity > 0.6)
			color = vec4(0.8, 0.8, 0.8, 1.0);
		else if (intensity > 0.3)
			color = vec4(0.5,0.5,0.5,1.0);
		else if (intensity > 0.1)
			color = vec4(0.35,0.35,0.35,1.0);
		else
			color = vec4(0.15,0.15,0.15,1.0);
	}
	/*if(fApplyTexture == 1)
		FragColor = texture(Texture, fTexCoord) * vec4(fLightColor * intensity, 1.0f);
	else*/
	FragColor = vec4(fLightColor * vec3(color),1.0);//vec4( color, 1.0f);
}									 