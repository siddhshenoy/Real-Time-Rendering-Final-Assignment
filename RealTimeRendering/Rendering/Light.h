#pragma once

#include <glm/glm/glm.hpp>

typedef struct LightData
{
	float AmbientStrength;
	float DiffuseStrength;
	float SpecularStrength;
	float SpecularPower;
}
LightData;

class Light
{
private:
	glm::vec3 mDirection;
private:
	void LoadLightDefaults();
	
public:
	glm::vec3 Position;
	glm::vec3 Color;
	LightData LightData;
public:
	Light();
	Light(const glm::vec3& position);
	Light(const glm::vec3& position, const glm::vec3& direction);
	Light(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color);
	void SetPosition(const glm::vec3& position);
	void SetColor(const glm::vec3& color);
	void SetDirection(const glm::vec3& direction);
	glm::vec3 GetPosition();
	glm::vec3 GetColor();
	glm::vec3 GetDirection();
	~Light();
};

