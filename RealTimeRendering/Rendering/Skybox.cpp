#include "Skybox.h"

Skybox::Skybox()
{
}
void Skybox::LoadSkyboxFromData(const std::vector<std::string>& FaceList) {



	//Then load the skybox
	mVertices = std::vector<glm::vec3>({   
		glm::vec3(-1.0f, 1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f, 1.0f, -1.0f),
		glm::vec3(-1.0f, 1.0f, -1.0f),

		glm::vec3(-1.0f, -1.0f, 1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f, 1.0f, -1.0f),
		glm::vec3(-1.0f, 1.0f, -1.0f),
		glm::vec3(-1.0f, 1.0f, 1.0f),
		glm::vec3(-1.0f, -1.0f, 1.0f),

		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),

		glm::vec3(-1.0f, -1.0f, 1.0f),
		glm::vec3(-1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, -1.0f, 1.0f),
		glm::vec3(-1.0f, -1.0f, 1.0f),

		glm::vec3(-1.0f, 1.0f, -1.0f),
		glm::vec3(1.0f, 1.0f, -1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-1.0f, 1.0f, 1.0f),
		glm::vec3(-1.0f, 1.0f, -1.0f),

		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f, 1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f, 1.0f),
		glm::vec3(1.0f, -1.0f, 1.0)
	});

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);
	int img_width, img_height, img_nchannels;
	unsigned char* data;
	int i = 0;
	for (std::string face : FaceList) {
		//std::cout << "[SKYBOX]: Loading face: " << face << std::endl;
		spdlog::info("[SKYBOX]: Loading face: " + face);
		data = stbi_load(face.c_str(), &img_width, &img_height, &img_nchannels, 4);
		if (!data) {
			spdlog::error("[SKYBOX ERROR]: Could not face data from '" + face + "'");
			stbi_image_free(data);
		}
		else {
			//std::cout << "[SKYBOX]: Loaded face '" << face << "' width: " << img_width << " height: " << img_height << std::endl;
			spdlog::info("[SKYBOX]: Loaded face '" + face + "' width: " + std::to_string(img_width) + " height: " + std::to_string(img_height));
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data
			);
			i++;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(glm::vec3), &mVertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
}
Skybox::Skybox(const std::string& path)
{
	std::vector<std::string> face_names;
	for (std::string FaceName : mFaceList) {
		face_names.push_back(path + "/" + FaceName + ".jpg"); //.jpg will be the default extension
	}
	LoadSkyboxFromData(face_names);
}

Skybox::Skybox(const std::string& path, const std::string& extension)
{
	std::vector<std::string> face_names;
	for (std::string FaceName : mFaceList) {
		face_names.push_back(path + "/" + FaceName + "." + extension); //.jpg will be the default extension
	}
	LoadSkyboxFromData(face_names);
}

void Skybox::SetShader(Shader* shader) {
	mShader = shader;
}

void Skybox::BindSkyboxTexture()
{

}
void Skybox::UseSkybox(Camera* cam) {
	glDepthFunc(GL_LEQUAL);
	mShader->UseShader();
	glBindVertexArray(mVAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);
	glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
	mViewProj = cam->GetTransformForSkybox();
	mShader->SetUniformMatrix(const_cast<GLchar*>("viewproj"), 1, GL_FALSE, &mViewProj[0][0]);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}

Skybox::~Skybox()
{
}
