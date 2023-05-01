#pragma once

#include <Rendering/Transform.h>
#include <Rendering/Camera.h>
#include <Rendering/Material.h>
#include <Rendering/Mesh.h>

#include <Structures/Map.h>

class Object : public Transform
{
private:
	Material* mMaterial;
	//Temporary variable to hold all the child objects..
	std::vector<Transform*> mChildTransforms;
	std::vector<Object*> mChildObjects;
public:
	bool HasParentRendered;
	bool (*mPreRenderingFunction)(Object*);
	Map<std::string, float> KeyList;
public:
	Object();
	void AssignMaterial(Material* material);
	Material* GetMaterial();
	void Render(Camera* cam);

	//Temporary function, can we do something like "Transform" handles all the child objects but it categorizes it using a hashmap
	//Then we can extract the list as and when needed? Something of those sorts
	void AddChildObject(Object* object);
	void AddChildTransform(Transform* transform);

	std::vector<Object*> GetChildObjects();
	std::vector<Transform*> GetChildTransforms();

	~Object();
};

