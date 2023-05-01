#pragma once

#include <string>
#include <iostream>
#include <vector>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/quaternion.hpp> 
#include <glm/glm/gtx/quaternion.hpp>

class Transform
{
private:
	
	std::string mName;
	bool isTransformSet;
public:
	Transform* mParent;
	std::vector<Transform*> mChildren;
	glm::mat4 mTransform;
	glm::vec3 Translation;
	glm::vec3 Rotation;
	glm::vec3 Scale;
public:
	Transform();
	void SetTranslation(const glm::vec3& translation);
	void SetRotation(const glm::vec3& rotations);
	void SetScale(const glm::vec3& scale);

	glm::vec3 GetTranslation();
	glm::vec3 GetWorldScale();
	glm::vec3 GetWorldTranslation();
	glm::vec3 GetWorldRotation();

	void TranslateX(float x);
	void TranslateY(float y);
	void TranslateZ(float z);

	void RotateX(float angle);
	void RotateY(float angle);
	void RotateZ(float angle);

	void ScaleX(float scale);
	void ScaleY(float scale);
	void ScaleZ(float scale);

	glm::vec3 const GetPosition();
	glm::vec3 const GetRotation();
	glm::vec3 const GetScale();

	glm::mat4 GetTransform();

	void SetParent(Transform* transform);
	Transform* GetParent();

	void SetTransformation(const glm::mat4& transform);

	void AddChild(Transform* transform);
	std::vector<Transform*> GetChildren();

	void SetName(const char* str);
	std::string GetName();

	~Transform();
};

