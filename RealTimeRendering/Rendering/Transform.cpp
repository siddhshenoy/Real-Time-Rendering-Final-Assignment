#include "Transform.h"

Transform::Transform()
{
    isTransformSet = false;
    mTransform = glm::mat4(1.0f);
    mParent = nullptr;
    Translation = glm::vec3(0.0f, 0.0f, 0.0f);
    Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    Scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

void Transform::SetTranslation(const glm::vec3& translation)
{
    Translation = translation;
}

void Transform::SetRotation(const glm::vec3& rotations)
{
    Rotation = rotations;
}

void Transform::SetScale(const glm::vec3& scale)
{
    Scale = scale;
}

glm::vec3 Transform::GetTranslation()
{
    return Translation;
}
glm::vec3 Transform::GetWorldScale() {
    if (mParent != nullptr) {
        glm::mat4 resultMatrix = this->GetTransform();
        glm::vec3 resultScale = glm::vec3(resultMatrix[0][0], resultMatrix[1][1], resultMatrix[2][2]);
        return resultScale;
    }
    return Scale;
}
glm::vec3 Transform::GetWorldTranslation()
{
    if (mParent != nullptr) {
        glm::mat4 resultMatrix = this->GetTransform();
        glm::vec3 resultTranslate = glm::vec3(resultMatrix[3][0], resultMatrix[3][1], resultMatrix[3][2]);
        return resultTranslate;
    }
    else
        return Translation;
}

void Transform::TranslateX(float x)
{
    Translation.x = x;
}

void Transform::TranslateY(float y)
{
    Translation.y = y;
}

void Transform::TranslateZ(float z)
{
    Translation.z = z;
}

void Transform::RotateX(float angle)
{
    Rotation.x = angle;
}

void Transform::RotateY(float angle)
{
    Rotation.y = angle;
}

void Transform::RotateZ(float angle)
{
    Rotation.z = angle;
}

void Transform::ScaleX(float scale)
{
    Scale.x = scale;
}

void Transform::ScaleY(float scale)
{
    Scale.y = scale;
}

void Transform::ScaleZ(float scale)
{
    Scale.z = scale;
}

glm::vec3 const Transform::GetPosition()
{
    return Translation;
}

glm::vec3 const Transform::GetRotation()
{
    return Rotation;
}

glm::vec3 const Transform::GetScale()
{
    return Scale;
}

glm::mat4 Transform::GetTransform()
{
    if (!isTransformSet) {
        mTransform = glm::mat4(1.0f);
        mTransform = glm::translate(mTransform, Translation);
        mTransform = glm::rotate(mTransform, glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        mTransform = glm::rotate(mTransform, glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        mTransform = glm::rotate(mTransform, glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        //std::cout << "Transform: " << mName << std::endl;
        //std::cout << "In Rot: " << Rotation.x << ", " << Rotation.y << ", " << Rotation.z << std::endl;
        //glm::quat rot_q(glm::vec3(glm::radians(Rotation.x), glm::radians(Rotation.y), glm::radians(Rotation.z)));
        //std::cout << "out Rot: " << Rotation.x << ", " << Rotation.y << ", " << Rotation.z << std::endl;
        //mTransform *= glm::toMat4(rot_q);
        mTransform = glm::scale(mTransform, Scale);
    }
    if (mParent != nullptr) {
        return (mParent->GetTransform() * mTransform);
    }
    return mTransform;
}
void Transform::SetTransformation(const glm::mat4& transform) {
    mTransform = transform;
}
void Transform::AddChild(Transform* transform)
{
    transform->SetParent(this);
    mChildren.push_back(transform);
}
std::vector<Transform*> Transform::GetChildren()
{
    return mChildren;
}
void Transform::SetName(const char* str)
{
    mName = std::string(str);
}

std::string Transform::GetName()
{
    return mName;
}

glm::vec3 Transform::GetWorldRotation() {
    glm::vec3 result_rotation = glm::vec3(0.0f);
    Transform* current = this;
    //We do a walk up the list of parent-child hierarchy to find the influencing rotations...
    while (current != nullptr) {
        result_rotation += current->Rotation;
        current = current->mParent;
 
    }
    return result_rotation;
}
Transform* Transform::GetParent() {
    return mParent;
}
void Transform::SetParent(Transform* transform) {
    mParent = transform;
}

Transform::~Transform()
{
}
