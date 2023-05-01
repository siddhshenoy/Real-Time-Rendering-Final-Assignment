#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"

Texture::Texture()
{
	mTextureID = 0;
	mMinificationType = GL_NEAREST;
	mMagnificationType = GL_LINEAR;
	mLODBias = 0.0f;
}

Texture::Texture(const char* texture_path)
{
	LoadTexture(texture_path);
}

void Texture::LoadTexture(const char* texture_path)
{

	glGenTextures(1, &mTextureID);

	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	GLint width, height, channel;
	unsigned char* data = stbi_load(texture_path, &width, &height, &channel, 4);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cerr << "Could not load the texture from path '" << texture_path << "'" << std::endl;
	}
	stbi_image_free(data);
}

void Texture::BindTexture(int bindLevel)
{
	//std::cout << "Binding texture on bind level: " << bindLevel << std::endl;
	glActiveTexture(GL_TEXTURE0 + bindLevel);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mMinificationType);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mMagnificationType);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, mLODBias);
	
}
void Texture::SetMinificationType(GLuint type)
{
	mMinificationType = type;
}
void Texture::SetMagnificationType(GLuint type)
{
	mMagnificationType = type;
}
void Texture::SetLODBias(GLfloat lod_bias)
{
	mLODBias = lod_bias;
}

void Texture::UnbindTexture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &mTextureID);
}
