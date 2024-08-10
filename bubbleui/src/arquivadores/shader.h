#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <utility>
#include <glm/gtc/type_ptr.hpp>

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
    void use() const;
    // utility uniform functions
    void compilar(const char* vertexPath, const char* fragmentPath);
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, float r, float g, float b) const;
    void setVec2(const std::string& name, float r, float g) const;
    void setMat4(const std::string& name, const float* value) const;
    void setMat3(const std::string& name, const float* value) const;
    bool checkCompileErrors(unsigned int shader, const std::string& type);
    bool checkLinkErrors(unsigned int shader);
};

#endif