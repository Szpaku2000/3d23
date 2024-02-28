//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <vector>

#include "Application/application.h"
#include "Application/utils.h"

#include "glad/gl.h"
#include "glm/glm.hpp"
#include "camera.h"
#include "camera_controler.h"
#include "Engine/Light.h"
#include <Engine/Mesh.h>
#include <Engine/Material.h>

class SimpleShapeApplication : public xe::Application
{
public:
    SimpleShapeApplication(int width, int height, std::string title, bool debug) : Application(width, height, title, debug) {}

    void init() override;

    void frame() override;

    void framebuffer_resize_callback(int w, int h) override;

    void scroll_callback(double xoffset, double yoffset) override;

    void set_camera(Camera *camera) { camera_ = camera; }
    Camera *camera() { return camera_; }
    ~SimpleShapeApplication() {
        if (camera_) {
            delete camera_;
        }
    }

    void set_controler(CameraControler *controler) { controler_ = controler; }

    void mouse_button_callback(int button, int action, int mods) override;

    void cursor_position_callback(double x, double y) override;

    void add_submesh(xe::Mesh* mesh) {
        meshes_.push_back(mesh);
    }

    void add_light(const xe::PointLight &p_light) {
        p_lights_.push_back(p_light);
    }

private:
    GLuint vao_;
    int w, h;

    GLuint u_pvm_buffer_;
    GLuint transformations;
    Camera *camera_;
    CameraControler *controler_;

    std::vector<xe::Mesh*> meshes_;

    GLuint lights_buffer_;

    glm::vec3 ambient_;
    std::vector<xe::PointLight> p_lights_;
};