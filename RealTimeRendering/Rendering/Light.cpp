#include "Light.h"

void Light::LoadLightDefaults() {
    this->LightData.AmbientStrength = 0.5f;
    this->LightData.DiffuseStrength = 0.5f;
    this->LightData.SpecularStrength = 1.0f;
    this->LightData.SpecularPower = 32.0f;
}

Light::Light()
{
    this->LoadLightDefaults();
}

Light::Light(const glm::vec3& position)
{
    this->LoadLightDefaults();
    Position = position;
}

Light::Light(const glm::vec3& position, const glm::vec3& direction)
{
    this->LoadLightDefaults();
    Position = position;
    mDirection = direction;
}

Light::Light(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color)
{
    this->LoadLightDefaults();
    Position = position;
    mDirection = direction;
    Color = color;
}

void Light::SetPosition(const glm::vec3& position)
{
    Position = position;
}

void Light::SetColor(const glm::vec3& color)
{
    Color = color;
}

void Light::SetDirection(const glm::vec3& direction)
{
    mDirection = direction;
}

glm::vec3 Light::GetPosition()
{
    return Position;
}

glm::vec3 Light::GetColor()
{
    return Color;
}

glm::vec3 Light::GetDirection()
{
    return mDirection;
}

Light::~Light()
{
}
