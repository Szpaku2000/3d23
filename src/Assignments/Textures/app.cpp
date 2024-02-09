//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>

#include "Application/utils.h"
#include "Engine/mesh_loader.h"
#include "Engine/Material.h"

#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#define STB_IMAGE_IMPLEMENTATION  1

#include "3rdParty/stb/stb_image.h"


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

    std::vector<GLfloat> vertices = {
            0.0f, 1.0f, 0.0f, 0.0f, 1.0f,       //  0   LT
            0.0f, 1.0f, 0.0f, 1.0f, 1.0f,       //  1   RT
            0.0f, 1.0f, 0.0f, 1.0f, 0.0f,       //  2   RD
            0.0f, 1.0f, 0.0f, 0.0f, 0.0f,       //  3   LD
            -0.5f, 0.0f, -0.5f, 0.5f, 0.809f,   //  4   T
            -0.5f, 0.0f, 0.5f, 0.191f, 0.5f,    //  5   L
            0.5f, 0.0f, -0.5f, 0.809f, 0.5f,    //  6   R
            0.5f, 0.0f, 0.5f, 0.5f, 0.191f,     //  7   D
    };

    std::vector<GLushort> indexes = {
            5,0,4,
            5,7,3,
            7,6,2,
            6,4,1,
            4,7,5,
            4,6,7
    };

    stbi_set_flip_vertically_on_load(true);
    GLint width, height, channels;
    auto texture_file = std::string(ROOT_DIR) + "/Models/multicolor.png";
    auto img = stbi_load(texture_file.c_str(), &width, &height, &channels, 0);
    if (!img) {
        std::cerr << "Could not read image from file `{}'" << texture_file;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0u);

    xe::ColorMaterial::init();

    float strength = 0.8;
    float color[3] = { 1.0, 1.0, 1.0 };

    auto pyramid = xe::load_mesh_from_obj(std::string(ROOT_DIR) + "/Models/pyramid.obj",
                                            std::string(ROOT_DIR) + "/Models");
    add_submesh(pyramid);

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

