#pragma once


#include <glew/GL/glew.h>
#include <glm/glm/glm.hpp>
#include <vector>
#include <string>

#include <Rendering/Structures.h>

typedef struct Model {
	std::vector<Vertex> VertexData;
	std::vector<GLuint> IndexData;
	std::vector<Model*> ChildModels;
}
Model;