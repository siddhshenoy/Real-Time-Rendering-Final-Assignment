#pragma once

#include <iostream>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtx/transform.hpp>

enum class ProjectionType {
	PERSPECTIVE,
	ORTHOGRAPHIC
};

typedef struct PerspectiveParams {
	float Fov;
	float AspectRatio;
	float Near;
	float Far;
}
PerspectiveParams;

typedef struct OrthographicParams {
	float Left;
	float Right;
	float Top;
	float Bottom;
	float Near;
	float Far;
}
OrthographicParams;

class Projection
{
protected:
	glm::mat4 mProjectionMatrix;
	ProjectionType mProjectionType;
	PerspectiveParams perspectiveParams;
	OrthographicParams orthographicParams;

public:
	Projection();

	void SetProjectionType(ProjectionType projType);
	ProjectionType GetProjectionType();

	void InitializePerspectiveParameters(float fov, float aspectRatio, float near, float far);
	void InitializeOrthographicParameters(float left, float right, float bottom, float top, float near, float far);

	glm::mat4 GetProjectionMatrix();
	~Projection();
};

