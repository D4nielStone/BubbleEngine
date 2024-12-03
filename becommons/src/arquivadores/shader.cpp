
/** @copyright Copyright (c) 2024 Daniel Oliveira */

#include "assets/shaders_na_memoria.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.hpp"
#include "src/depuracao/debug.hpp"
#include <src/util/includes.hpp>
#include <filesystem>
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

// Mapeia os shaders na memória para facilitar o acesso aos shaders embutidos
const std::map<std::string, const char*> shader_memoria{
    {"quad.vert", quad_vert},
    {"quad.frag", quad_frag},
    {"imagem.vert", imagem_vert},
    {"imagem.frag", imagem_frag},
    {"texto.vert", texto_vert},
    {"texto.frag", texto_frag},
    {"skybox.vert", skybox_vert},
    {"skybox.frag", skybox_frag},
    {"phong.vert", phong_vert},
    {"phong.frag", phong_frag}
};

std::vector<std::pair<std::pair<const char*, const char*>, unsigned int>> shaders;

ShaderException::ShaderException(const char* msg) : msg_(msg) {}

const char* ShaderException::what() const noexcept {
    return msg_.c_str();
}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    compilar(vertexPath, fragmentPath);
}

void Shader::compilar(const char* vertexPath, const char* fragmentPath) {
    // Verifica se o shader já foi compilado
    for (const auto& shader : shaders) {
        if (shader.first.first == vertexPath && shader.first.second == fragmentPath) {
            ID = shader.second;
            return;
        }
    }

    // Inicializa o GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw ShaderException("Falha ao iniciar o GLAD");
    }

    // Cria o programa shader
    try {
        ID = glCreateProgram();
    }
    catch (const std::exception& e) {
        std::cerr << "Erro ao criar shader_program: " << e.what() << std::endl;
    }

    const char* vertexShaderSource{};
    const char* fragmentShaderSource{};

    // Abre e lê os arquivos de shader
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile, fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    if (!std::filesystem::exists(vertexPath) || !std::filesystem::exists(fragmentPath))
    {
        // Verifica se o shader está na memória
        if (shader_memoria.find(std::filesystem::path(vertexPath).filename().string()) != shader_memoria.end()) {
            vertexShaderSource = shader_memoria.at(std::filesystem::path(vertexPath).filename().string());
        }
        else
            return;
        if (shader_memoria.find(std::filesystem::path(fragmentPath).filename().string()) != shader_memoria.end()) {
            fragmentShaderSource = shader_memoria.at(std::filesystem::path(fragmentPath).filename().string());
        }
        else
            return;
    }
    else
    {
        try {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;

            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
            vertexShaderSource = vertexCode.c_str();
            fragmentShaderSource = fragmentCode.c_str();
        }
        catch (const std::ifstream::failure& e)
        {
            std::cerr << e.what() << "\n";
        }
    }
    // Compilação do Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    if (!checkCompileErrors(vertexShader, "VERTEX")) return;

    // Compilação do Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    if (!checkCompileErrors(fragmentShader, "FRAGMENT")) return;

    // Vinculação e linkagem dos shaders
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    if (!checkLinkErrors(ID)) return;

    // Limpeza
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    
    shaders.push_back({ {vertexPath, fragmentPath}, ID });
}

void Shader::use() {
    glUseProgram(ID);
    shader_atual = this;
}

void Shader::setBool(const std::string& name, const bool& value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, const float& value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setInt(const std::string& name, const int& value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, const float* value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}

void Shader::setMat3(const std::string& name, const float* value) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}

void Shader::setCor(const std::string& name, const Color& cor) const {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), cor.r, cor.g, cor.b, cor.a);
}

void Shader::setVec3(const std::string& name, const float& r, const float& g, const float& b) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), r, g, b);
}

void Shader::setVec2(const std::string& name, const float& r, const float& g) const {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), r, g);
}

bool Shader::checkCompileErrors(unsigned int shader, const std::string& type) {
    GLint success;
    GLchar infoLog[1024];

    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << ": " << infoLog << std::endl;
            return false;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR: " << infoLog << std::endl;
            return false;
        }
    }
    return true;
}

bool Shader::checkLinkErrors(unsigned int program) {
    GLint success;
    GLchar infoLog[1024];

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 1024, NULL, infoLog);
        std::cerr << "PROGRAM_LINKING_ERROR: " << infoLog << std::endl;
        return false;
    }
    return true;
}