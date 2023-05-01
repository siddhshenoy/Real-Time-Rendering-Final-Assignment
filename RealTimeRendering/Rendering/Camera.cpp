#include "Camera.h"

Camera::Camera()
{
    mPosition = glm::vec3(0.0f);
    mUp = glm::vec3(0.0f, 1.0f, 0.0f);
    mRight = glm::vec3(1.0f, 0.0f, 0.0f);
    Position = &mPosition;
    Up = &mUp;
    Right = &mRight;
}

void Camera::Translate(const glm::vec3& position)
{
    Camera::LookAt(position, (position + mForward), mUp);
}

void Camera::LookAt(const glm::vec3& eyePosition, const glm::vec3& centre, const glm::vec3& up)
{
    mPosition = eyePosition;
    mCentre = centre;
    mUp = up;
    mForward = (centre - mPosition);
    mRight = glm::normalize(glm::cross(mForward, mUp));
    mTransform = glm::lookAt(mPosition, (mPosition + mForward), mUp);

}
void Camera::RotateX(float angle) {
    glm::vec3 right = glm::normalize(glm::cross(mUp, mForward));
    mForward = glm::vec3(glm::normalize(glm::rotate(angle, right) * glm::vec4(mForward, 0.0)));
    mUp = glm::normalize(glm::cross(mForward, right));
    mRight = glm::normalize(glm::cross(mForward, mUp));
    this->LookAt(mPosition, (mPosition + mForward), mUp);
}
void Camera::RotateY(float angle) {
    static const glm::vec3 UP(0.0f, 1.0f, 0.0f);
    glm::mat4 rotation = glm::rotate(angle, UP);
    mForward = glm::vec3(glm::normalize(rotation * glm::vec4(mForward, 0.0)));
    mUp = glm::vec3(glm::normalize(rotation * glm::vec4(mUp, 0.0)));
    mRight = glm::normalize(glm::cross(mForward, mUp));
    this->LookAt(mPosition, (mPosition + mForward), mUp);
}
const glm::vec3 Camera::GetRight() {
    
    return mRight;
}
const glm::vec3 Camera::GetForward()
{
    return mForward;
}
glm::vec3 const Camera::GetTranslation() {
    return mPosition;
}
glm::mat4 Camera::GetTransform()
{
    return GetProjectionMatrix() * mTransform;
}

glm::mat4 Camera::GetTransformForSkybox()
{
    glm::mat4 cam_matrix = glm::mat4(glm::mat3(mTransform));
    return GetProjectionMatrix() * cam_matrix;
}

Camera::~Camera()
{
}
