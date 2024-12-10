#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <glm/gtc/type_ptr.hpp>
#include "assets/shaders_na_memoria.hpp"
#include "src/util/cor.hpp"

inline std::vector<std::pair<std::pair<const char*, const char*>, unsigned int>> shaders;
// Mapeia os shaders na memória para facilitar o acesso aos shaders embutidos
inline const std::map<std::string, const char*> shader_memoria{
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

class ShaderException : public std::exception
{
public:
    ShaderException(const char* msg);
    virtual const char* what() const noexcept override;
private:
    std::string msg_;
};

class Shader
{
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader() { compilar("assets/shaders/phong.vert", "assets/shaders/phong.frag"); };
    Shader(const char* vertexPath, const char* fragmentPath);
    // use/activate the shader
    void use();
    // utility uniform functions
    void compilar(const char* vertexPath, const char* fragmentPath);
    void setBool(const std::string& name, const bool& value) const;
    void setInt(const std::string& name, const int& value) const;
    void setFloat(const std::string& name, const float& value) const;
    void setCor(const std::string& name, const Cor& cor) const;
    void setVec3(const std::string& name, const float& r, const float& g, const float& b) const;
    void setVec2(const std::string& name, const float& r, const float& g) const;
    void setMat4(const std::string& name, const float* value) const;
    void setMat3(const std::string& name, const float* value) const;
    bool checkCompileErrors(unsigned int shader, const std::string& type);
    bool checkLinkErrors(unsigned int shader);
};