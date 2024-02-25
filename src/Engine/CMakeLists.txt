cmake_minimum_required(VERSION 3.15)
project(Engine CXX)

add_compile_definitions(PROJECT_NAME="${PROJECT_NAME}" PROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

add_library(engine
        Mesh.h Mesh.cpp
        Material.h
        mesh_loader.h mesh_loader.cpp
        PhongMaterial.h PhongMaterial.cpp
        utils.h utils.cpp
        ColorMaterial.cpp
        ColorMaterial.h
        texture.cpp
        texture.h
)

target_link_libraries(engine PUBLIC objreader)