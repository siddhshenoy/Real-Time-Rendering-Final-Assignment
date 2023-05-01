#pragma once

#include <glew/GL/glew.h>



#include <Rendering/Transform.h>

#include <Rendering/Shader.h>
#include <Rendering/Camera.h>
#include <Rendering/Texture.h>

#include <spdlog/spdlog.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtx/transform.hpp>
class Sprite : public Transform
{
public:
	static int gSpriteCounter;
	static std::vector<Sprite*> gSpriteList;
	static void RemoveSpriteFromList(Sprite* sprite);
private:
	GLuint mVAO;
	GLuint mVBO[2];
	Shader* TextShader;
	Texture* texture;
	bool IsVisible;
public:
	glm::vec4 mColor;
public:
	Sprite();
	Sprite(const std::string& sprite_image_path);
	void AssignShader(Shader* shader);
	void LoadSpriteFromPath(const std::string& sprite_image_path);
	void SetTexture(GLuint texture) {
		this->texture->SetTexture(texture);
	}
	GLuint GetTexture() {
		return this->texture->GetTextureID();
	}
	Texture* GetTextureInstance() {
		return this->texture;
	}
	void SetColor(const glm::vec4& color);
	glm::vec4 GetColor();
	void SetVisible(bool set);
	void Render(Camera* cam);
	void RenderBlank();
	~Sprite();
};

