#pragma once
#include "Component.h"
#include "Shader.h"
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glad/glad.h>

class Transform : public Component {
private:
    glm::vec3 position;
    glm::vec3 rotation; // Euler angles
    glm::vec3 scale;
    Shader shader;
public:
    Transform()
        : position(0.f, 0.f, 0.f), rotation(0.0f), scale(0.5f) {
        Name = "Transform";
    }

    void update() override {
        shader.setMat4("model", glm::value_ptr(GetModelMatrix()));
    }
    void load() override {
        std::cout << ">> Transform loaded\n";
    }
    glm::mat4 GetModelMatrix() const {
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotationMatrix = glm::toMat4(glm::quat(rotation));
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

        return translationMatrix * rotationMatrix * scaleMatrix;
    }

    glm::vec3 GetPosition() const { return position; }
    glm::vec3 GetRotation() const { return rotation; }
    glm::vec3 GetScale() const { return scale; }

    void SetPosition(const glm::vec3& newPosition) { position = newPosition; }
    void SetRotation(const glm::vec3& newRotation) { rotation = newRotation; }
    void SetScale(const glm::vec3& newScale) { scale = newScale; }
    void Rotate(const glm::vec3& newRotation) { rotation += newRotation; }

    void setShader(Shader s) { shader = s; };

};


