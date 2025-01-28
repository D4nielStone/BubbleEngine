cmake_minimum_required(VERSION 3.10)
project(golf)

set(CMAKE_CXX_STANDARD 20)

add_library(becommons STATIC IMPORTED)
set_target_properties(becommons PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/../becommons/build/libbecommons.a)

find_package(Lua REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)
find_package(RapidJSON REQUIRED)
find_package(Bullet REQUIRED )
find_package(Freetype REQUIRED)
find_package(assimp REQUIRED)

add_executable(golf 
    src/main.cpp
)

add_library(glad STATIC /usr/include/glad/glad.c)
target_link_libraries(golf PRIVATE glad)

target_include_directories(golf PRIVATE 
    ../becommons
    /usr/include/lua5.1
    /usr/include/glm
    /usr/include/GLFW
    /usr/include/glad
    /usr/include/assimp
    /usr/include
    /usr/include/rapidjson
    /usr/include/bullet
    /usr/include/freetype2
)

target_link_libraries(golf PRIVATE 
 becommons 
 BulletDynamics
 LinearMath 
 BulletCollision
 Bullet3Common
 glfw 
 assimp
)


find_library(BULLET_DYNAMICS_LIBRARY BulletDynamics)
find_library(BULLET_COLLISION_LIBRARY BulletCollision)
find_library(BULLET_LINEAR_MATH_LIBRARY LinearMath)

# Definir _DEBUG para Debug e NDEBUG para Release
add_compile_definitions(
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release,RelWithDebInfo,MinSizeRel>:NDEBUG>
)