#include "Projection.h"

Projection::Projection()
{
	mProjectionMatrix = glm::mat4(1.0f);
	mProjectionType = ProjectionType::PERSPECTIVE;
}

void Projection::SetProjectionType(ProjectionType projType)
{
	mProjectionType = projType;
}

ProjectionType Projection::GetProjectionType()
{
	return mProjectionType;
}

void Projection::InitializePerspectiveParameters(float fov, float aspectRatio, float near, float far)
{
	perspectiveParams.Fov = fov;
	perspectiveParams.AspectRatio = aspectRatio;
	perspectiveParams.Near = near;
	perspectiveParams.Far = far;
	/*std::cout << __FUNCTION__ << ": Initializing perspective parameters "
		<< "(Fov: " << fov
		<< " | AspectRatio: " << aspectRatio
		<< " | Near: " << near
		<< " | Far: " << far
		<< ")" << std::endl;
		*/
}
void Projection::InitializeOrthographicParameters(float left, float right, float bottom, float top, float near, float far) {
	orthographicParams.Left = left;
	orthographicParams.Right = right;
	orthographicParams.Top = top;
	orthographicParams.Bottom = bottom;
	orthographicParams.Near = near;
	orthographicParams.Far = far;
}
glm::mat4 Projection::GetProjectionMatrix()
{
	switch (mProjectionType)
	{
		case ProjectionType::PERSPECTIVE: {
			mProjectionMatrix = glm::perspective(
				perspectiveParams.Fov,
				perspectiveParams.AspectRatio,
				perspectiveParams.Near,
				perspectiveParams.Far
			);
			break;
		}
		case ProjectionType::ORTHOGRAPHIC: {
			mProjectionMatrix = glm::ortho(
				orthographicParams.Left,
				orthographicParams.Right,
				orthographicParams.Bottom,
				orthographicParams.Top,
				orthographicParams.Near,
				orthographicParams.Far
			);
			break;
		}
		default:
		{
			mProjectionMatrix = glm::mat4(1.0f);
		}
	}
	return mProjectionMatrix;
}

Projection::~Projection()
{
}
