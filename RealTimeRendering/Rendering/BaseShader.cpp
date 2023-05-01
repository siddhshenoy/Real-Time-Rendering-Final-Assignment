#include "BaseShader.h"

BaseShader::BaseShader()
{
	mShaderID = -1;
	mShaderName = "";
}

BaseShader::BaseShader(const char* shaderName, const char* ShaderFilePath, GLenum ShaderType)
{
	mShaderID = -1;
	mShaderName = "";
	CreateBasicShader(shaderName, ShaderFilePath, ShaderType);
}

void BaseShader::CreateBasicShader(const char* shaderName, const char* ShaderFilePath, GLenum ShaderType)
{
	Utils::ReadFullFile(ShaderFilePath, mShaderSource);
	if (mShaderSource != "" && mShaderSource.length() > 0) {
		mShaderID = glCreateShader(ShaderType);
		if (mShaderID != -1) {
			const char* data = mShaderSource.c_str();
			int length;
			glShaderSource(mShaderID, 1, &data, &length);
			glCompileShader(mShaderID);
			GLint param;
			glGetShaderiv(mShaderID, GL_COMPILE_STATUS, &param);
			if (param != GL_TRUE) {
				char buffer[1024];
				glGetShaderInfoLog(mShaderID, sizeof(buffer), NULL, buffer);
				std::cerr << "-----------------------------------------------------------------------\n"
					<< "SHADER COMPILATION FAILED" << (strlen(shaderName) > 0 ? (": " + std::string(shaderName)) + "\n" : "\n")
					<< "ERROR: " << buffer
					<< "-----------------------------------------------------------------------"
					<< std::endl;
						
				//printf("%sShader compilation error: %s\n", shaderName, buffer);
			}
		}
	}
}

GLuint BaseShader::GetShaderID()
{
	return mShaderID;
}

void BaseShader::DeleteShader()
{
	if (mShaderID != -1) {
		glDeleteShader(mShaderID);
	}
}

BaseShader::~BaseShader()
{
}
