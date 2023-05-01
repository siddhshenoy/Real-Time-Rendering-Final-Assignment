/*
	The RotateX and RotateY functions were extracted from 'thebennybox' github project.
	Link for his camera video:
	https://www.youtube.com/watch?v=e3sc72ZDDpo
*/

#pragma once

#include <glm/glm/glm.hpp>
#include <glm/glm/gtx/transform.hpp>

#include <Rendering/Projection.h>

class Camera : public Projection
{
private:
	glm::vec3 mPosition;
	glm::vec3 mCentre;
	glm::vec3 mForward;
	glm::vec3 mRight;
	glm::vec3 mUp;
	glm::mat4 mTransform;
public:
	glm::vec3* Position;
	glm::vec3* Forward;
	glm::vec3* Up;
	glm::vec3* Right;
public:
	Camera();
	void Translate(const glm::vec3& position);
	void LookAt(const glm::vec3& eyePosition, const glm::vec3& centre, const glm::vec3& up);
	void RotateX(float angle);
	void RotateY(float angle);
	const glm::vec3 GetForward();
	const glm::vec3 GetTranslation();
	const glm::vec3 GetRight();
	glm::mat4 GetTransform();
	glm::mat4 GetTransformForSkybox();
	~Camera();
};

