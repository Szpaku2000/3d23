//
// Created by kasz2 on 24.02.2024.
//

#include <string>
#include <iostream>
#include "texture.h"
#include "glad/gl.h"
#include "3rdParty/stb/stb_image.h"

namespace xe {
    GLuint create_texture(const std::string &name) {

        stbi_set_flip_vertically_on_load(true);
        GLint width, height, channels;
        auto img = stbi_load(name.c_str(), &width, &height, &channels, 0);
        if (!img) {
            std::cerr << "Could not read image from file" << name;
            return 0;
        }
        GLenum format = GL_RGB;
        if (channels == 3)
            format = GL_RGB;
        else if (channels == 4) {
            format = GL_RGBA;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, img);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0u);

        return texture;
    }
}