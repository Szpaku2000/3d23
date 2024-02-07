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

    xe::ColorMaterial::init();

//     A vector containing the x,y,z vertex coordinates for the triangle.
    std::vector<GLfloat> vertices = {
            -0.5f, 0.0f ,  0.5f,   //FL violet     0
            0.5f,  0.0f ,  0.5f,   //FR v          1
            -0.5f, 0.0f , -0.5f,   //BL v          2
            0.5f,  0.0f,  -0.5f,   //BR v          3
            0.0f,  1.0f ,  0.0f,   //T red         4
            };

    std::vector<GLushort> indexes = {
            0,2,1,      // first bottom violet
            1,2,3,      // second bottom violet
            4,0,1,      // side red
            4,2,0,      // side green
            4,1,3,      // side blue
            4,3,2       // side yellow
    };

    float strength = 0.8;
    float color[3] = { 1.0, 1.0, 1.0 };

    auto pyramid = new xe::Mesh;
    pyramid->allocate_vertex_buffer(vertices.size() * sizeof(GLfloat), GL_STATIC_DRAW);
    pyramid->load_vertices(0, vertices.size() * sizeof(GLfloat), vertices.data());
    pyramid->allocate_index_buffer(indexes.size() * sizeof(GLushort), GL_STATIC_DRAW);
    pyramid->load_indices(0, indexes.size() * sizeof(GLushort), indexes.data());
    pyramid->vertex_attrib_pointer(0, 3, GL_FLOAT, 3 * sizeof(GLfloat), 0);
    pyramid->add_submesh(0, 6, new xe::ColorMaterial({ 1.0f, 0.0f, 1.0f, 1.0f }));
    pyramid->add_submesh(6, 9, new xe::ColorMaterial({1.0f, 0.0f, 0.0f, 1.0f}));
    pyramid->add_submesh(9, 12, new xe::ColorMaterial({0.0f, 1.0f, 0.0f, 1.0f}));
    pyramid->add_submesh(12, 15, new xe::ColorMaterial({0.0f, 0.0f, 1.0f, 1.0f}));
    pyramid->add_submesh(15, 18, new xe::ColorMaterial({1.0f, 1.0f, 0.0f, 1.0f}));
    add_submesh(pyramid);

//    // Generating the buffer and loading the vertex data into it.
//    GLuint v_buffer_handle;
//    glGenBuffers(1, &v_buffer_handle);
//    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle));
//    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//    // Indeces
//    GLuint v_buffer_handle_indexes;
//    glGenBuffers(1, &v_buffer_handle_indexes);
//    OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, v_buffer_handle_indexes));
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(GLfloat), indexes.data(), GL_STATIC_DRAW);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Uniforms colors
    GLuint v_buffer_handle_uniforms_colors;
    glGenBuffers(1, &v_buffer_handle_uniforms_colors);
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, v_buffer_handle_uniforms_colors));
    glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(float), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, v_buffer_handle_uniforms_colors);

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &strength);
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(float), 4 * sizeof(float), &color);

    // Uniforms position
    glGenBuffers(1, &u_pvm_buffer_);
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_));
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_pvm_buffer_);

    // This setups a Vertex Array Object (VAO) that  encapsulates
    // the state of all vertex buffers needed for rendering
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
//    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, v_buffer_handle_indexes);

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
    set_controler(new CameraControler(camera()));
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
    glBindVertexArray(0);
    for (auto m : meshes_) {
        m->draw();
    }
}

void  SimpleShapeApplication::mouse_button_callback(int button, int action, int mods) {
    Application::mouse_button_callback(button, action, mods);

    if (controler_) {
        double x, y;
        glfwGetCursorPos(window_, &x, &y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            controler_->LMB_pressed(x, y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
            controler_->LMB_released(x, y);
    }

}

void  SimpleShapeApplication::cursor_position_callback(double x, double y) {
    Application::cursor_position_callback(x, y);
    if (controler_) {
        controler_->mouse_moved(x, y);
    }
}

