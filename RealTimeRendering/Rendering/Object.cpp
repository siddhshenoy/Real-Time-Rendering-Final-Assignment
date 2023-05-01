#include "Object.h"

Object::Object()
{
	mMaterial = nullptr;
	mPreRenderingFunction = nullptr;
	HasParentRendered = true;
}

void Object::AssignMaterial(Material* material)
{
	mMaterial = material;
}

Material* Object::GetMaterial() {
	return mMaterial;
}

void Object::Render(Camera* cam)
{
	bool preRenderingResult = true;
	if (mPreRenderingFunction != nullptr) {
		preRenderingResult = mPreRenderingFunction(this);
		HasParentRendered = preRenderingResult;
	}
	if (mParent != nullptr) {
		preRenderingResult = ((Object*)mParent)->HasParentRendered;
	}
	if (preRenderingResult) {
		
		glm::mat4 mTempTransform;
		Shader* shader = mMaterial->GetShader();
		if (shader != nullptr) {
			//Texture* tex = mMaterial->GetTexture();
			std::map<std::string, MaterialTextureData*> texMaps = mMaterial->GetTextureMaps();
			shader->UseShader();
			if (texMaps.size() > 0) {
				for (std::map<std::string, MaterialTextureData*>::iterator it = texMaps.begin(); it != texMaps.end(); it++) {
					//std::cout << "Applying texture: " << it->first << std::endl;
					it->second->texture->BindTexture(it->second->bindLevel);
				}
			}
			shader->SetUniformMatrix("model", 1, GL_FALSE, &mTransform[0][0]);
			for (Transform* T : mChildren) {
				Mesh* mesh = (Mesh*)T;
				mTempTransform = cam->GetTransform() * mesh->GetTransform();
				shader->SetUniformMatrix(const_cast<GLchar*>("transform"), 1, GL_FALSE, &mTempTransform[0][0]);
				mesh->Render(shader);
			}
			//shader->DisposeShader();
		}
	}
}
void Object::AddChildObject(Object* o) {
	mChildObjects.push_back(o);
	o->SetParent(this);
}
void Object::AddChildTransform(Transform* transform) {
	mChildTransforms.push_back(transform);
	transform->SetParent(this);
}
std::vector<Object*> Object::GetChildObjects() {
	return mChildObjects;

}
std::vector<Transform*> Object::GetChildTransforms() {
	return mChildTransforms;
}
Object::~Object()
{
}
