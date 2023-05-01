#pragma once

#include <glm/glm/glm.hpp>

#include <Rendering/Shader.h>
#include <Rendering/Texture.h>

enum class MaterialPropertyType {
	//Primitive type
	INT,
	FLOAT,
	//Primitive type arrays
	FLOAT_ARR,
	INT_ARR,
	//Complex datatypes
	VEC2,
	VEC3,
	MATRIX3,
	MATRIX4
};

typedef struct MaterialProperty {
	MaterialPropertyType PropertyType;
	void* Value;
} MaterialProperty;

typedef struct MaterialTextureData {
	Texture* texture;
	int bindLevel;
};

class Material
{
private:
	Shader* mShader;
	Texture* mTexture;
	std::map<std::string, MaterialProperty*> mMaterialProperties;
	std::map<std::string, MaterialTextureData*> mTextureMaps;
public:
	Material();
	void AttachShader(Shader* shader);
	void AttachTexture(Texture* texture);
	void AttachTexture(const std::string& texture_name, Texture* texture, int bindLevel);
	void SetProperty(const std::string& PropertyName, void* Val, MaterialPropertyType type);
	MaterialProperty* GetProperty(const std::string& PropertyName);
	bool HasProperty(const std::string& PropertyName);
	void ApplyMaterial();
	Shader* GetShader();
	Texture* GetTexture();
	Texture* GetTextureFromName(const std::string& name);
	std::map<std::string, MaterialTextureData*> GetTextureMaps();
	~Material();
};

