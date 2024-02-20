//
// Created by Piotr Białas on 02/11/2021.
//

#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>

#include "Application/utils.h"

namespace xe {

    GLuint create_texture(const std::string &name);

    class Material {
    public:
        virtual void bind() = 0;

        virtual void unbind() {};
    };

    class ColorMaterial : public Material {
    public:
        ColorMaterial(const glm::vec4 color) : color_(color), texture_(0), texture_unit_(0) {}

        ColorMaterial(const glm::vec4 color, GLuint texture, GLuint texture_unit) :
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


