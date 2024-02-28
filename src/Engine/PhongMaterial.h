//
// Created by Piotr Białas on 20/11/2021.
//

#pragma once

#include "Material.h"

#include <string>

namespace xe {
    class PhongMaterial : public Material {
    public:
        PhongMaterial(const glm::vec4 color) : Kd_(color), texture_(0), texture_unit_(0) {}

        PhongMaterial(const glm::vec4 color, GLuint texture) :
                Kd_(color), texture_(texture), texture_unit_(0) {}

        PhongMaterial(const glm::vec4 color, GLuint texture, GLuint texture_unit) :
                Kd_(color), texture_(texture), texture_unit_(texture_unit) {}

        PhongMaterial(const glm::vec4 color, const glm::vec3 ambient, const glm::vec3 specular, float specularStrength) :
                Kd_(color), Ka_(ambient), Ks_(specular), Ns(specularStrength), texture_(0), texture_unit_(0) {}

        void bind();
        void unbind();

        static void init();

        static GLuint program() { return shader_; }

        GLuint get_texture_unit() const {
            return texture_unit_;
        }

        void set_texture_unit(GLuint textureUnit) {
            texture_unit_ = textureUnit;
        }

        void set_texture(GLuint tex) { texture_ = tex; }
        GLuint get_texture() const {
            return texture_;
        }

        static void set_ambient(const glm::vec3 ambient) {
            glUseProgram(program());
            glUniform3f(uniform_ambient_, ambient.x, ambient.y, ambient.z);
            glUseProgram(0);
        }

    private:
        GLfloat Ns;
        static GLuint shader_;
        static GLuint material_uniform_buffer_;

        static GLint uniform_ambient_;
        static GLint uniform_map_Kd_location_;
        GLuint texture_;
        GLuint texture_unit_;
        glm::vec4 Kd_;
        glm::vec3 Ka_;
        glm::vec3 Ks_;
    };

}




