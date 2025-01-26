cmake_minimum_required(VERSION 3.10)

# Nome do projeto
project(becommons)

# Definir a versão do projeto
set(PROJECT_VERSION "1.0")

# Definir como uma biblioteca estática
add_library(becommons STATIC)

# Adicionar as diretivas para compilar com C++21
set(CMAKE_CXX_STANDARD 21)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Adicionar diretórios de include das bibliotecas
include_directories(
    /usr/include/lua5.1
    /usr/include/glm
    /usr/include/GLFW
    /usr/include/glad
    /usr/include/assimp
    /usr/include/FreeImage
    /usr/include/rapidjson
    /usr/include/bullet
    /usr/include/freetype2
)

# Adicionar as dependências da biblioteca
find_package(Lua REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)
find_package(RapidJSON REQUIRED)
find_package(Bullet REQUIRED)
find_package(Freetype REQUIRED)

# Adicionar os arquivos de código fonte da biblioteca
target_sources(becommons PRIVATE
    src/api/api_lua.cpp
    /usr/include/glad/glad.c
    src/arquivadores/fonte.cpp
    src/arquivadores/imageloader.cpp
    src/arquivadores/shader.cpp
    src/arquivadores/modelo.cpp
    src/componentes/camera.cpp
    src/componentes/codigo.cpp
    src/componentes/fisica.cpp
    src/componentes/transformacao.cpp
    src/depuracao/debug.cpp
    src/entidades/entidade.cpp
    src/inputs/inputs.cpp
    src/nucleo/fase.cpp
    src/nucleo/sistema_de_codigo.cpp
    src/nucleo/sistema_de_fisica.cpp
    src/nucleo/sistema_de_interface.cpp
    src/nucleo/sistema_de_renderizacao.cpp
    src/nucleo/sistema.cpp
    src/util/malha.cpp
)

# Linkar as bibliotecas necessárias
target_link_libraries(becommons
    PRIVATE
    lua5.1
    glfw
    glad
    assimp
    FreeImage
    rapidjson
    Bullet
    freetype
)

# Se você ainda estiver incluindo GLM manualmente, remova isso
# Não precisa linkar GLM com target_link_libraries, pois é uma biblioteca header-only

# Instalar a biblioteca (se necessário)
install(TARGETS becommons DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)