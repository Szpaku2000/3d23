//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>

#include "Application/utils.h"
#include "Engine/mesh_loader.h"
#include "Engine/PhongMaterial.h"

#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#define STB_IMAGE_IMPLEMENTATION  1

#include "3rdParty/stb/stb_image.h"
#include "Engine/ColorMaterial.h"
#include "Engine/PhongMaterial.h"

struct SceneLights {
    xe::PointLight light[24];
    int num_lights;
};

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

    xe::PhongMaterial::init();

    float strength = 0.8;
    float color[3] = { 1.0, 1.0, 1.0 };

    auto pyramid = xe::load_mesh_from_obj(std::string(PROJECT_DIR) + "/Models/square.obj",
                                            std::string(PROJECT_DIR) + "/Models");
    add_submesh(pyramid);

    // Uniforms colors
    GLuint v_buffer_handle_uniforms_colors;
    glGenBuffers(1, &v_buffer_handle_uniforms_colors);
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, v_buffer_handle_uniforms_colors));
    glBufferData(GL_UNIFORM_BUFFER, 16 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, v_buffer_handle_uniforms_colors);

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GLfloat), &strength);
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(GLfloat), 4 * sizeof(GLfloat), &color);

    // Uniforms position
    glGenBuffers(1, &u_pvm_buffer_);
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_));
    glBufferData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_pvm_buffer_);

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 6.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);

    // Transformation
    glGenBuffers(1, &transformations);
    glBindBuffer(GL_UNIFORM_BUFFER, transformations);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) + sizeof(glm::mat3), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, transformations);

    add_light(*new xe::PointLight({ 0.0, 0.3, 0.5 }, { 0.0, 0.0, 1.0 }, { 1.0, 1.0, 1.0 }));
    add_light(*new xe::PointLight({ -0.3, -0.3, 0.5 }, { 0.25, 1.0, 0.0 }, { 1.0, 1.0, 1.0 }));
    add_light(*new xe::PointLight({ 0.3, -0.3, 0.5 }, { 1.0, 0.0, 0.75 }, { 1.0, 1.0, 1.0 }));
//    xe::PhongMaterial::set_ambient({ 0.5f, 0.5f, 0.5f });

    glGenBuffers(1, &lights_buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER, lights_buffer_);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(SceneLights), nullptr, GL_STATIC_DRAW);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, 3, lights_buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER, lights_buffer_);

    // This setups a Vertex Array Object (VAO) that  encapsulates
    // the state of all vertex buffers needed for rendering
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

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

    glm::mat4 VM = camera()->view() * glm::mat4(1.0);
    auto R = glm::mat3(VM);
    auto N = glm::mat3(glm::cross(R[1], R[2]), glm::cross(R[2], R[0]), glm::cross(R[0], R[1]));
    glBindBuffer(GL_UNIFORM_BUFFER, lights_buffer_);
    SceneLights scene_lights{};
    scene_lights.num_lights = p_lights_.size();

    for(int i = 0; i < scene_lights.num_lights; i++) scene_lights.light[i] = p_lights_[i];

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(SceneLights), &scene_lights);
    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, transformations);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &VM[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, transformations);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat3), &N[0]);
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

