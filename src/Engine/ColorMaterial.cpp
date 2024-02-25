//
// Created by kasz2 on 24.02.2024.
//

#include "ColorMaterial.h"
 namespace xe {

     GLuint ColorMaterial::color_uniform_buffer_ = 0u;
     GLuint ColorMaterial::shader_ = 0u;
     GLint  ColorMaterial::uniform_map_Kd_location_ = 0;

     void ColorMaterial::bind() {
         GLint use_map_Kd = 0;
         if (texture_ > 0){
             use_map_Kd = 1;
             glUniform1i(uniform_map_Kd_location_, texture_unit_);
             glActiveTexture(GL_TEXTURE0 + texture_unit_);
             glBindTexture(GL_TEXTURE_2D, texture_);
         }
         glBindBufferBase(GL_UNIFORM_BUFFER, 0, color_uniform_buffer_);
         glUseProgram(program());
         glBindBuffer(GL_UNIFORM_BUFFER, color_uniform_buffer_);
         glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &color_[0]);
         glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(GLint), &use_map_Kd);
         glBindBuffer(GL_UNIFORM_BUFFER, 0u);

     }


     void ColorMaterial::init() {
         auto program = xe::utils::create_program(
                 {{GL_VERTEX_SHADER,   std::string(PROJECT_DIR) + "/shaders/color_vs.glsl"},
                  {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/color_fs.glsl"}});
         if (!program) {
             std::cerr << "Invalid program" << std::endl;
             exit(-1);
         }

         shader_ = program;

         uniform_map_Kd_location_ = glGetUniformLocation(shader_, "map_Kd");
         if (uniform_map_Kd_location_ == -1) {
             std::cerr << "Cannot get uniform map_Kd location";
         }

         glGenBuffers(1, &color_uniform_buffer_);

         glBindBuffer(GL_UNIFORM_BUFFER, color_uniform_buffer_);
         glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) + sizeof(GLint), nullptr, GL_STATIC_DRAW);
         glBindBuffer(GL_UNIFORM_BUFFER, 0u);
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

     GLuint ColorMaterial::get_texture() const {
         return texture_;
     }

     void ColorMaterial::set_texture(GLuint texture) {
         texture_ = texture;
     }

     GLuint ColorMaterial::get_texture_unit() const {
         return texture_unit_;
     }

     void ColorMaterial::set_texture_unit(GLuint textureUnit) {
         texture_unit_ = textureUnit;
     }


}