/*
	Reference: https://learnopengl.com/Advanced-OpenGL/Cubemaps
*/

#pragma once

#include <string>
#include <vector>

#include <glew/GL/glew.h>
#include <stb/stb_image.h>
#include <glm/glm/glm.hpp>

#include <Rendering/Shader.h>
#include <Rendering/Camera.h>

#include <spdlog/spdlog.h>

class Skybox
{
private:
	Shader* mShader;
	GLuint mTextureID;
	GLuint mVAO, mVBO;
	std::vector<glm::vec3> mVertices;
	std::vector<std::string> mFaceList = std::vector<std::string>({
			"right",
			"left",
			"top",
			"bottom",
			"front",
			"back"
		});

	glm::mat4 mViewProj;
private:
	void LoadSkyboxFromData(const std::vector<std::string>& FaceList);
public:
	Skybox();
	Skybox(const std::string& path);
	Skybox(const std::string& path, const std::string& extension);
	void SetShader(Shader* shader);
	void UseSkybox(Camera* cam);
	void BindSkyboxTexture();
	~Skybox();
};

