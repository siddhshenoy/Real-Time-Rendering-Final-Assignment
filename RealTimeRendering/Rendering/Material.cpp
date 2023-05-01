#include "Material.h"

Material::Material()
{
}

void Material::AttachShader(Shader* shader)
{
    mShader = shader;
}

void Material::AttachTexture(Texture* texture)
{
    //mTexture = texture;
    MaterialTextureData* mat_tex_data = new MaterialTextureData;
    mat_tex_data->texture = texture;
    mat_tex_data->bindLevel = 0;
    mTextureMaps.insert(std::pair<std::string, MaterialTextureData*>("Default Texture", mat_tex_data));
}
void Material::AttachTexture(const std::string& texture_name, Texture* texture, int bindLevel) {
    MaterialTextureData* mat_tex_data = new MaterialTextureData;
    mat_tex_data->texture = texture;
    mat_tex_data->bindLevel = bindLevel;
    mTextureMaps.insert(std::pair<std::string, MaterialTextureData*>(texture_name, mat_tex_data));
}
void Material::SetProperty(const std::string& PropertyName, void* Val, MaterialPropertyType type)
{
    if (HasProperty(PropertyName)) {
        mMaterialProperties[PropertyName]->PropertyType = type;
        mMaterialProperties[PropertyName]->Value = Val;
    }
    else {
        MaterialProperty* property = new MaterialProperty;
        property->Value = Val;
        property->PropertyType = type;
        mMaterialProperties.insert(
            std::pair<std::string, MaterialProperty*>(PropertyName, property)
        );
    }
}

MaterialProperty* Material::GetProperty(const std::string& PropertyName)
{
    if (HasProperty(PropertyName))
        return mMaterialProperties[PropertyName];
    else
        return nullptr;
}
bool Material::HasProperty(const std::string& PropertyName) {
    return (mMaterialProperties.find(PropertyName) != mMaterialProperties.end());
}
void Material::ApplyMaterial()
{
    //if (mTexture != nullptr) mTexture->BindTexture();
    if (mTextureMaps.size() > 0) {
        for (std::map<std::string, MaterialTextureData*>::iterator it = mTextureMaps.begin(); it != mTextureMaps.end(); it++) {
            std::cout << "Applying texture: " << it->first << std::endl;
            it->second->texture->BindTexture(it->second->bindLevel);
        }
    }
    if (mShader != nullptr) mShader->UseShader();
    std::string property_name = "";
    for (
        std::map<std::string, MaterialProperty*>::iterator it = mMaterialProperties.begin();
        it != mMaterialProperties.end();
        it++
        ) {
        property_name = it->first;
        MaterialProperty* prop = it->second;
        switch (prop->PropertyType) {
            case MaterialPropertyType::FLOAT: {
                float value = *static_cast<GLfloat*>(prop->Value);
                std::cout << "FLOAT: PropertyName: '" << property_name << "' Value: " << value << std::endl;
                mShader->SetUniform1f(
                    const_cast<GLchar*>(property_name.c_str()),
                    value
                );
                break;
            }
            case MaterialPropertyType::VEC3: {
                glm::vec3 result = *static_cast<glm::vec3*>(prop->Value);
                std::cout << "VEC3: PropertyName: '" << property_name << "' value: " << result.x << ", " << result.y << ", " << result.z << std::endl;
                mShader->SetUniform3f(
                    const_cast<GLchar*>(property_name.c_str()),
                    result.x,
                    result.y,
                    result.z
                );
                break;
            }
            case MaterialPropertyType::VEC2: {
                glm::vec2 result = *static_cast<glm::vec2*>(prop->Value);
                std::cout << "VEC2: PropertyName: '" << property_name << "' value: " << result.x << ", " << result.y  << std::endl;
                mShader->SetUniform2f(
                    const_cast<GLchar*>(property_name.c_str()),
                    result.x,
                    result.y
                );
                break;
            }
            case MaterialPropertyType::MATRIX4: {
                glm::mat4 result = *static_cast<glm::mat4*>(prop->Value);
                std::cout << "MATRIX4: PropertyName: '" << property_name << "' value: " << std::endl;
                for (int i = 0; i < 4; i++)
                    for (int j = 0; j < 4; j++)
                        std::cout << result[i][j] << "\t";
                    std::cout << std::endl;

                mShader->SetUniformMatrix(
                    const_cast<GLchar*>(property_name.c_str()),
                    1,
                    GL_FALSE,
                    &result[0][0]
                );
                break;
            }
        }
    }
}
std::map<std::string, MaterialTextureData*> Material::GetTextureMaps()
{
    return mTextureMaps;
}
Shader* Material::GetShader()
{
    return mShader;
}

Texture* Material::GetTexture()
{
    return mTexture;
}
Texture* Material::GetTextureFromName(const std::string& texture_name) {
    return mTextureMaps[texture_name]->texture;
}
Material::~Material()
{
}
