//
// Created by Piotr Białas on 20/11/2021.
//

#include "PhongMaterial.h"

#include "Application/utils.h"
#include "Engine/utils.h"

namespace xe {


    GLuint PhongMaterial::material_uniform_buffer_ = 0u;
    GLint PhongMaterial::uniform_ambient_ = 0;
    GLuint PhongMaterial::shader_ = 0u;
    GLint  PhongMaterial::uniform_map_Kd_location_ = 0;

    void PhongMaterial::bind() {
        glUseProgram(program());
        GLint use_map_Kd = 0;
        if (texture_ > 0){
            use_map_Kd = 1;
            glUniform1i(uniform_map_Kd_location_, texture_unit_);
            glActiveTexture(GL_TEXTURE0 + texture_unit_);
            glBindTexture(GL_TEXTURE_2D, texture_);
        }
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, material_uniform_buffer_);
        glUseProgram(program());
        glBindBuffer(GL_UNIFORM_BUFFER, material_uniform_buffer_);

        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &Kd_[0]);
        glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4), sizeof(GLint), &use_map_Kd);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec3), &Ks_[0]);
        glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), sizeof(glm::vec4), &Ka_[0]);
        glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4) + sizeof(GLfloat), sizeof(GLfloat), &Ns);
        glBindBuffer(GL_UNIFORM_BUFFER, 0u);

    }

    void PhongMaterial::init() {
        auto program = xe::utils::create_program(
                {{GL_VERTEX_SHADER,   std::string(PROJECT_DIR) + "/shaders/phong_vs.glsl"},
                 {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/phong_fs.glsl"}});
        if (!program) {
            std::cerr << "Invalid program" << std::endl;
            exit(-1);
        }

        shader_ = program;

        uniform_map_Kd_location_ = glGetUniformLocation(shader_, "map_Kd");
        if (uniform_map_Kd_location_ == -1) {
            std::cerr << "Cannot get uniform map_Kd location";
        }

        glGenBuffers(1, &material_uniform_buffer_);

        glBindBuffer(GL_UNIFORM_BUFFER, material_uniform_buffer_);
        glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4) + 2 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0u);

        uniform_map_Kd_location_ = glGetUniformLocation(shader_, "map_Kd");
        uniform_ambient_ = glGetUniformLocation(shader_, "ambient_light");

#if __APPLE__
        auto u_modifiers_index = glGetUniformBlockIndex(program, "Color");
        if (u_modifiers_index == -1) {
            std::cerr << "Cannot find Color uniform block in program" << std::endl;
        } else {
            glUniformBlockBinding(program, u_modifiers_index, 0);
        }
#endif

#if __APPLE__
        auto u_transformations_index = glGetUniformBlockIndex(program, "Transformations");
        if (u_transformations_index == -1) {
            std::cerr << "Cannot find Transformations uniform block in program" << std::endl;
        } else {
            glUniformBlockBinding(program, u_transformations_index, 1);
        }
#endif
    }

    void PhongMaterial::unbind() {
        glBindBuffer(GL_UNIFORM_BUFFER, 0u);
        glBindTexture(GL_TEXTURE_2D, 0u);
    }

}