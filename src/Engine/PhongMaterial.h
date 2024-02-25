//
// Created by Piotr Białas on 20/11/2021.
//

#pragma once

#include "Material.h"

#include <string>

namespace xe {
    class PhongMaterial : public Material {
    public:
        PhongMaterial(const glm::vec4 color) : color_(color), texture_(0), texture_unit_(0) {}

        PhongMaterial(const glm::vec4 color, GLuint texture, GLuint texture_unit) :
                color_(color), texture_(texture), texture_unit_(texture_unit) {}

        void bind();

        static void init();

        static GLuint program() { return shader_; }

        GLuint get_texture() const;

        GLuint get_texture_unit() const;

        void set_texture_unit(GLuint textureUnit);

        void set_texture(GLuint texture);

    private:
        static GLuint shader_;
        static GLuint color_uniform_buffer_;

        glm::vec4 color_ = glm::vec4(1.0, 0.0, 0.0, 1.0);

        static GLint uniform_map_Kd_location_;
        GLuint texture_;
        GLuint texture_unit_;
    };

}




