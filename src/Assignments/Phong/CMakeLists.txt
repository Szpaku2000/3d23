cmake_minimum_required(VERSION 3.15)
project(Phong)

add_compile_definitions(PROJECT_NAME="${PROJECT_NAME}" PROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(${PROJECT_NAME}
        main.cpp
        app.h
        app.cpp
        camera.h
        rotation.h
        camera_controler.h
)

target_link_libraries(${PROJECT_NAME} PUBLIC engine)
target_link_libraries(${PROJECT_NAME} PUBLIC )
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

