//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>

#include "Application/utils.h"

#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

void SimpleShapeApplication::init() {
    // A utility function that reads the shader sources, compiles them and creates the program object
    // As everything in OpenGL we reference program by an integer "handle".
    auto program = xe::utils::create_program(
            {{GL_VERTEX_SHADER,   std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"},
             {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"}});

    if (!program) {
        std::cerr << "Invalid program" << std::endl;
        exit(-1);
    }

//     A vector containing the x,y,z vertex coordinates for the triangle.
    std::vector<GLfloat> vertices = {
            -0.5f , 0.0f , 0.5f, 1.0f, 0.0f, 1.0f,      //FL violet     0
            0.5f , 0.0f , 0.5f, 1.0f, 0.0f, 1.0f,       //FR v          1
            -0.5f, 0.0f , -0.5f, 1.0f, 0.0f, 1.0f,      //BL v          2
            0.5f, 0.0f, -0.5f, 1.0f, 0.0f, 1.0f,        //BR v          3
            0.0f, 1.0f , 0.0f, 1.0f, 0.0f, 0.0f,        //T red         4
            -0.5f , 0.0f , 0.5f, 1.0f, 0.0f, 0.0f,      //FL red        5
            0.5f , 0.0f , 0.5f, 1.0f, 0.0f, 0.0f,       //FR r          6
            0.0f, 1.0f , 0.0f, 0.0f, 1.0f, 0.0f,        //T green       7
            -0.5f , 0.0f , 0.5f, 0.0f, 1.0f, 0.0f,      //FL green      8
            -0.5f, 0.0f , -0.5f, 0.0f, 1.0f, 0.0f,      //BL g          9
            0.0f, 1.0f , 0.0f, 0.0f, 0.0f, 1.0f,        //T blue        10
            0.5f , 0.0f , 0.5f, 0.0f, 0.0f, 1.0f,       //FR b          11
            0.5f, 0.0f, -0.5f, 0.0f, 0.0f, 1.0f,        //BR b          12
            0.0f, 1.0f , 0.0f, 1.0f, 1.0f, 0.0f,        //T yellow       13
            -0.5f, 0.0f , -0.5f, 1.0f, 1.0f, 0.0f,      //BL y          14
            0.5f, 0.0f, -0.5f, 1.0f, 1.0f, 0.0f,        //BR y          15
            };

    std::vector<GLushort> indexes = {
            0,2,1,      // first bottom violet
            1,2,3,      // second bottom violet
            4,5,6,      // side red
            7,9,8,      // side green
            10,11,12,   // side blue
            13,15,14    // side yellow
    };

    float strength = 0.8;
    float color[3] = { 1.0, 1.0, 1.0 };


    // Generating the buffer and loading the vertex data into it.
    GLuint v_buffer_handle;
    glGenBuffers(1, &v_buffer_handle);
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle));
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Indeces
    GLuint v_buffer_handle_indexes;
    glGenBuffers(1, &v_buffer_handle_indexes);
    OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, v_buffer_handle_indexes));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(GLfloat), indexes.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Uniforms colors
    GLuint v_buffer_handle_uniforms_colors;
    glGenBuffers(1, &v_buffer_handle_uniforms_colors);
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, v_buffer_handle_uniforms_colors));
    glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(float), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, v_buffer_handle_uniforms_colors);

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &strength);
    glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(float), 4 * sizeof(float), &color);

    // Uniforms position
    glGenBuffers(1, &u_pvm_buffer_);
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_));
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_pvm_buffer_);

    // This setups a Vertex Array Object (VAO) that  encapsulates
    // the state of all vertex buffers needed for rendering
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, v_buffer_handle_indexes);

    // This indicates that the data for attribute 0 should be read from a vertex buffer.
    glEnableVertexAttribArray(0);
    // and this specifies how the data is layout in the buffer.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));

    // color attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //end of vao "recording"

    // Setting the background color of the rendering window,
    // I suggest not to use white or black for better debuging.
    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);

    // This setups an OpenGL vieport of the size of the whole rendering window.
    auto[w, h] = frame_buffer_size();
    glViewport(0, 0, w, h);

    set_camera(new Camera);
    std::tie(w, h) = frame_buffer_size();
    camera()->perspective(glm::pi<float>()/4.0f, (float)w/h, 0.1f, 100.0f);
    camera()->look_at(glm::vec3(0.0f, 3.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, 1.0, 0.0));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glUseProgram(program);
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0,0,w,h);
    camera()->set_aspect(w/h);
}

void SimpleShapeApplication::scroll_callback(double xoffset, double yoffset) {
    Application::scroll_callback(xoffset, yoffset);
    camera()->zoom(yoffset / 30.0f);
}

//This functions is called every frame and does the actual rendering.
void SimpleShapeApplication::frame() {
    glm::mat4 PVM = camera()->projection() * camera()->view();

    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // Binding the VAO will setup all the required vertex buffers.
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid*>(0));
    glBindVertexArray(0);
}



