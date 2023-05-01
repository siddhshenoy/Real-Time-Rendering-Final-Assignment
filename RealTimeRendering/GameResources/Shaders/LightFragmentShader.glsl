//
//  Phong Shading model
//  Link: 
//

#version 330 core

out vec4 FragColor;

uniform vec3 fLightColor;

void main()
{   
    FragColor = vec4(fLightColor, 1.0f);
}									 