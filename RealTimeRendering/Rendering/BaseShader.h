#pragma once

#include <iostream>
#include <glew/GL/glew.h>
#include <Core/Utils.h>

class BaseShader
{
private:
	std::string mShaderName;
	GLuint mShaderID;
	std::string mShaderSource;
public:
	BaseShader();
	BaseShader(const char* shaderName, const char* ShaderFilePath, GLenum ShaderType);
	void CreateBasicShader(const char* shaderName, const char* ShaderFilePath, GLenum ShaderType);
	GLuint GetShaderID();
	void DeleteShader();
	~BaseShader();
};

