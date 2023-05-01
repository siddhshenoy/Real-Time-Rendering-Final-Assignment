#include "Shader.h"

Shader::Shader()
{
	mProgramID = -1;
}

Shader::Shader(const std::vector<BaseShader*> Shaders)
{
	LoadShaders(Shaders);
}

void Shader::LoadShaders(const std::vector<BaseShader*> Shaders)
{
	for (BaseShader* shader : Shaders) {
		mShaders.push_back(shader);
	}

	mProgramID = glCreateProgram();
	if (mProgramID != -1) {
		for (BaseShader* shader : mShaders) {
			glAttachShader(mProgramID, shader->GetShaderID());
		}
		glLinkProgram(mProgramID);
		int param;
		glGetProgramiv(mProgramID, GL_LINK_STATUS, &param);
		if (param == GL_FALSE) {
			char buffer[1024];
			glGetProgramInfoLog(mProgramID, sizeof(buffer), NULL, buffer);
			std::cerr << "-----------------------------------------------------------------------\n"
				<< "PROGRAM LINKING FAILED\n"
				<< "ERROR: " << buffer
				<< "-----------------------------------------------------------------------"
				<< std::endl;
		}
	}
}

void Shader::UseShader()
{
	if (mProgramID != -1) {
		glUseProgram(mProgramID);
	}
}
void Shader::DisposeShader() {
	if (mProgramID != -1) {
		glUseProgram(0);
	}
}
GLint Shader::GetAttibLocation(GLchar* attribName) {
	if (mAttribMap.find(attribName) != mAttribMap.end())
	{
		return mAttribMap[attribName];
	} else {
		GLint attributeLocation = glGetAttribLocation(mProgramID, attribName);
		mAttribMap.insert(std::pair<GLchar*, GLint>(attribName, attributeLocation));
		return attributeLocation;
	}
}
GLint Shader::GetUniformLocation(GLchar* uniformName) {
	if (mUniformMap.find(uniformName) != mUniformMap.end()) {
		return mUniformMap[uniformName];
	}
	else {

		GLint uniformLocation = glGetUniformLocation(mProgramID, uniformName);
		mUniformMap.insert(std::pair<GLchar*, GLint>(uniformName, uniformLocation));
		return uniformLocation;
	}
}
void Shader::SetUniform1i(const char* uniformName, GLint value) {
	GLint location = GetUniformLocation(const_cast<GLchar*>(uniformName));
	if (location != -1) {
		glUniform1i(location, value);
	}
}
void Shader::SetUniform3f(const char* uniformName, const glm::vec3& floats)
{
	GLint location = GetUniformLocation(const_cast<GLchar*>(uniformName));
	if (location != -1) {
		glUniform3f(location, floats.x, floats.y, floats.z);
	}
}
void Shader::SetUniform3f(const char* uniformName, GLfloat v0, GLfloat v1, GLfloat v2) {
	GLint location = GetUniformLocation(const_cast<GLchar*>(uniformName));
	if (location != -1) {
		glUniform3f(location, v0, v1, v2);
	}
}
void Shader::SetUniform3fv(GLchar* uniformName, GLfloat* v0) {
	GLint location = GetUniformLocation(uniformName);
	if (location != -1) {
		glUniform3fv(location, 1, v0);
	}
}
void Shader::SetUniform4f(const char* uniformName, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
	GLint location = GetUniformLocation(const_cast<GLchar*>(uniformName));
	if (location != -1) {
		glUniform4f(location, v0, v1, v2, v3);
	}
}
void Shader::SetUniform1f(const char* uniformName, GLfloat value) {
	GLint location = GetUniformLocation(const_cast<GLchar*>(uniformName));
	if (location != -1) {
		glUniform1f(location, value);
	}
}
void Shader::SetUniform2f(const char* uniformName, GLfloat v0, GLfloat v1)
{
	GLint uniLoc = GetUniformLocation(const_cast<GLchar*>(uniformName));
	if (uniLoc != -1) {
		glUniform2f(uniLoc, v0, v1);
	}
}
void Shader::SetUniformMatrix(const char* uniformName, GLint count, GLboolean transpose, const GLfloat* data) {
	GLint uniLoc = GetUniformLocation(const_cast<GLchar*>(uniformName));
	if (uniLoc != -1) {
		glUniformMatrix4fv(uniLoc, count, transpose, data);
	}
}
void Shader::SetUniformMatrix3fv(const char* uniformName, GLint count, GLboolean transpose, const GLfloat* data)
{
	GLint uniLoc = GetUniformLocation(const_cast<GLchar*>(uniformName));
	if (uniLoc != -1) {
		glUniformMatrix3fv(uniLoc, count, transpose, data);
	}
}
GLuint Shader::GetProgramID() {
	return mProgramID;
}
Shader::~Shader()
{
	for (BaseShader* shader : mShaders) {
		glDetachShader(mProgramID, shader->GetShaderID());
	}
	if (mProgramID != -1) {
		glDeleteProgram(mProgramID);
	}
}
