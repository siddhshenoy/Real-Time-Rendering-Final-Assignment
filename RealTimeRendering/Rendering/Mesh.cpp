#include "Mesh.h"

Mesh::Mesh()
{
}

void Mesh::LoadMesh(Model* model)
{
	mModel = model;
	
	/*
	std::cout << "Total vertices loaded: " << model->VertexData.size() << std::endl;
	std::cout << "Total Indices loaded: " << model->IndexData.size() << std::endl;
	*/
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(2, mVBO);
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mModel->VertexData.size(), &mModel->VertexData[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(glm::vec3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(glm::vec3) + sizeof(glm::vec4)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(glm::vec3) + sizeof(glm::vec4) + sizeof(glm::vec3)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(glm::vec3) + sizeof(glm::vec4) + sizeof(glm::vec3) + sizeof(glm::vec2)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mModel->IndexData.size(), &mModel->IndexData[0], GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void Mesh::Render(Shader* shader, RenderType type)
{
	if (shader != nullptr) {
		glBindVertexArray(mVAO);
		switch (type) {
			case RenderType::ELEMENTS: {
				if (mModel->IndexData.size() > 0) {
					glDrawElements(GL_TRIANGLES, (GLsizei)mModel->IndexData.size(), GL_UNSIGNED_INT, 0);
				}
				break;
			}
			case RenderType::ARRAYS: {
				glDrawArrays(GL_TRIANGLES, 0, mModel->VertexData.size());
				break;
			}
		}
		glBindVertexArray(0);
	}
}

Mesh::~Mesh()
{
	glDeleteBuffers(2, mVBO);
	glDeleteVertexArrays(1, &mVAO);
}
