#pragma once
#include "Component.h"
#include "Shader.h"
#include "includes.h"
#include <glad/glad.h>

class Renderer : public Component{
private:
	Vertex mVertex;
    Material mMaterial;
    Shader shader;
	GLuint VAO, EBO, VBO;
    void setupBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // Calculate the total size needed for vertices, normals, and uvs
        size_t vertexSize = mVertex.vertices.size() * sizeof(float);
        size_t normalSize = mVertex.normals.size() * sizeof(float);
        size_t uvSize = mVertex.uvs.size() * sizeof(float);
        size_t totalSize = vertexSize + normalSize + uvSize;

        // Allocate buffer with total size
        glBufferData(GL_ARRAY_BUFFER, totalSize, nullptr, GL_STATIC_DRAW);

        // Fill buffer with vertices data
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize, &mVertex.vertices[0]);

        // Fill buffer with normals data if available
        if (!mVertex.normals.empty()) {
            glBufferSubData(GL_ARRAY_BUFFER, vertexSize, normalSize, &mVertex.normals[0]);
        }

        // Fill buffer with uvs data if available
        if (!mVertex.uvs.empty()) {
            glBufferSubData(GL_ARRAY_BUFFER, vertexSize + normalSize, uvSize, &mVertex.uvs[0]);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mVertex.indices.size() * sizeof(unsigned int), &mVertex.indices[0], GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normal attribute
        if (!mVertex.normals.empty()) {
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(vertexSize));
            glEnableVertexAttribArray(1);
        }

        // UV attribute
        if (!mVertex.uvs.empty()) {
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(vertexSize + normalSize));
            glEnableVertexAttribArray(2);
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    void drawModel() {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mVertex.indices.size()), GL_UNSIGNED_INT, 0);
    }
    void updateColors() {
        shader.setVec3("materialColor", mMaterial.diffuse.r, mMaterial.diffuse.g, mMaterial.diffuse.b);
        shader.setVec3("ambientColor", mMaterial.ambient.r, mMaterial.ambient.g, mMaterial.ambient.b);
        shader.setFloat("shininess", 32.f);
    }
public:
	Renderer() {
        Name = "Renderer";
		std::cout << ">> Renderer created\n";
    };
	Renderer(Vertex vertex, Material materials) : mVertex(vertex), mMaterial(materials){
        Name = "Renderer";
		std::cout << ">> Renderer created\n";
    };
	Renderer(Vertex vertex) : mVertex(vertex){
        Name = "Renderer";
		std::cout << ">> Renderer created\n";
    };
	void load() override {
        setupBuffers();
		std::cout << ">> Renderer loaded\n";
	}
	void update() override {
        updateColors();
        drawModel();
	}

    void setShader(Shader s) { shader = s; };
};