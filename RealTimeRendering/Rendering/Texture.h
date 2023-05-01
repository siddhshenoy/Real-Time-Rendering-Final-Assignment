#pragma once

#include <iostream>
#include <stb/stb_image.h>
#include <glew/GL/glew.h>

class Texture
{
private:
	GLuint mTextureID;
	GLfloat mLODBias;
	//GLuint mTextureAliasingType;
	GLuint mMinificationType;
	GLuint mMagnificationType;
public:
	Texture();
	Texture(const char* texture_path);
	void SetTexture(GLuint texture) {
		mTextureID = texture;
	}
	GLuint GetTextureID() {
		return mTextureID;
	}
	void LoadTexture(const char* texture_path);
	void BindTexture(int bindLevel = 0);
	void SetMinificationType(GLuint type);
	void SetMagnificationType(GLuint type);
	void SetLODBias(GLfloat lod_bias);
	void UnbindTexture();
	~Texture();
};
