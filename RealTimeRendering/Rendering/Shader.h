#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <glm/glm/glm.hpp>
#include <glew/GL/glew.h>

#include <Rendering/BaseShader.h>

#include <Core/Utils.h>
class Shader
{
private:
	GLuint mProgramID;
	std::vector<BaseShader*> mShaders;
	std::map<GLchar*, GLint> mAttribMap;
	std::map<GLchar*, GLint> mUniformMap;
public:
	Shader();
	Shader(const std::vector<BaseShader*> Shaders);
	void LoadShaders(const std::vector<BaseShader*> shaders);
	GLint GetAttibLocation(GLchar* attribName);
	GLint GetUniformLocation(GLchar* uniformName);
	void SetUniform1i(const char* uniformName, GLint value);
	void SetUniform1f(const char* uniformName, GLfloat value);
	void SetUniform2f(const char* uniformName, GLfloat v0, GLfloat v1);
	void SetUniform3f(const char* uniformName, const glm::vec3& floats);
	void SetUniform3f(const char* uniformName, GLfloat v0, GLfloat v1, GLfloat v2);
	void SetUniform3fv(GLchar* uniformName, GLfloat* v0);
	void SetUniform4f(const char* uniformName, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
	void SetUniformMatrix(const char* uniformName, GLint count, GLboolean transpose, const GLfloat* data);
	void SetUniformMatrix3fv(const char* uniformName, GLint count, GLboolean transpose, const GLfloat* data);
	GLuint GetProgramID();
	void UseShader();
	void DisposeShader();
	~Shader();
};

