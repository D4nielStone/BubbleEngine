#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.hpp"
#include "src/depuracao/debug.hpp"
#include <src/util/includes.hpp>

std::vector<std::pair<std::pair<const char*, const char*>, unsigned int>> shaders;
GLuint shader_atual;
ShaderException::ShaderException(const char* msg) : msg_(msg) {}
const char* ShaderException::what() const noexcept
{
    return msg_.c_str();
}
Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    compilar(vertexPath, fragmentPath);
}
void Shader::compilar(const char* vertexPath, const char* fragmentPath) {
    for (std::pair<std::pair<const char*, const char*>, unsigned int> shader : shaders)
    {
        if (shader.first.first == vertexPath && shader.first.second == fragmentPath)
        {
            ID = shader.second;
            return;
        }
    }
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw ShaderException("Falha ao iniciar o glad");
    }
    try {
        ID = glCreateProgram();
    }
    catch(std::exception& e){
        std::cerr << "Erro ao criar shader_program::" << e.what() << std::endl;
    }
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e) {
        std::cout << e.what() << "\n";
        return;
    }

    const char* vertexShaderSource = vertexCode.c_str();
    const char* fragmentShaderSource = fragmentCode.c_str();

    // Vertex Shader Compilation
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    if (!checkCompileErrors(vertexShader, "VERTEX")) {
        return;
    }

    // Fragment Shader Compilation
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    if (!checkCompileErrors(fragmentShader, "FRAGMENT")) {
        return;
    }

    // Shader Program Linking
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    if (!checkLinkErrors(ID)) {
        return;
    }

    // Clean up shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    shaders.push_back(std::pair(std::pair(vertexPath, fragmentPath), ID));
}   
void Shader::use() const {
    if (shader_atual != ID)
    {
        glUseProgram(ID);
        shader_atual = ID;
    }
}
void Shader::setBool(const std::string& name, const bool &value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, const float &value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setInt(const std::string& name, const int &value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setMat4(const std::string& name, const float* value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}
void Shader::setMat3(const std::string& name, const float* value) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}
void Shader::setCor(const std::string& name, const Color &cor) const {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), cor.r, cor.g, cor.b, cor.a);
}
void Shader::setVec3(const std::string& name, const float &r, const float &g, const float &b) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), r, g, b);
}
void Shader::setVec2(const std::string& name, const float &r, const float &g) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), r, g);
}
bool Shader::checkCompileErrors(unsigned int shader, const std::string& type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            return false;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- --\n";
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
        std::cout << "PROGRAM_LINKING_ERROR\n" + std::string(infoLog) + "\n-- -------------------------------------------------- - -- \n";
        return false;
    }
    return true;
}
