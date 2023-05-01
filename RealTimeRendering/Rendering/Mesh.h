#pragma once


#include <iostream>

#include <glew/GL/glew.h>

#include <Rendering/Camera.h>
#include <Rendering/Shader.h>
#include <Rendering/Transform.h>
#include <Rendering/Model.h>

class Mesh : public Transform
{
private:
	GLuint mVAO;
	GLuint mVBO[2];
public:
	Model* mModel;
public:
	Mesh();
	void LoadMesh(Model* model);
	void Render(Shader* shader, RenderType type = RenderType::ELEMENTS);
	~Mesh();
};

