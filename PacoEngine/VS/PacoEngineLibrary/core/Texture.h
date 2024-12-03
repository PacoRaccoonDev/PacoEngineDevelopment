#pragma once
//#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <iostream>
#include <glad/gl.h>

struct Texture
{
    GLuint id;
    int width, height;
};

namespace TextureFunctions
{
    static void Create(Texture& p_texture)
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &p_texture.id);
    }

    static void BindTextureUnit(Texture& p_texture, GLuint p_texture_unit)
    {

        glBindTextureUnit(p_texture_unit, p_texture.id);
    }

    static void Delete(Texture& p_texture)
    {
        glDeleteTextures(1, &p_texture.id);
    }

    static GLenum GetTextureFormat(GLenum& p_internal_format)
    {
        switch (p_internal_format)
        {
        case GL_RGBA8:
            return GL_RGBA;
        case GL_RGB8:
            return GL_RGB;
        case GL_R8:
            return GL_RED;
        case GL_DEPTH_COMPONENT:
            return GL_DEPTH_COMPONENT;
        default:
            fprintf(stderr, "Unsupported internal format: %u\n", p_internal_format);
            return 0; // Invalid format
        }
    }

    static void GenerateTextureFromFile(const char* p_path, Texture& p_texture, bool p_nearest_filtering = true)
    {
        stbi_set_flip_vertically_on_load(true);

        auto p_filter_type = GL_NEAREST;

        if (!p_nearest_filtering)
        {
            p_filter_type = GL_LINEAR;
        }

        // Load Image Data
        int p_channels;
        unsigned char* p_img = stbi_load(p_path, &p_texture.width, &p_texture.height, &p_channels, 0);

        if (p_img == NULL)
        {
            std::cerr << "Could not load the image" << std::endl;
            return;
        }

        Create(p_texture);
        glTextureStorage2D(p_texture.id, 1, GL_RGBA8, p_texture.width, p_texture.height);
        glTextureSubImage2D(p_texture.id, 0, 0, 0, p_texture.width, p_texture.height, GL_RGBA, GL_UNSIGNED_BYTE, p_img);

        glTextureParameteri(p_texture.id, GL_TEXTURE_MIN_FILTER, p_filter_type);
        glTextureParameteri(p_texture.id, GL_TEXTURE_MAG_FILTER, p_filter_type);
        glTextureParameteri(p_texture.id, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(p_texture.id, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glGenerateTextureMipmap(p_texture.id);

        // Free image data
        stbi_image_free(p_img);
    }

    // PixelStorageMode: 1 = for fonts | 4 = default
    static void GenerateTextureFromBitmap(Texture& p_texture, unsigned char& p_bitmap, int p_pixel_storage_mode, GLenum p_format, GLenum p_filter_type = GL_LINEAR)
    {
        //stbi_set_flip_vertically_on_load(true);

        GLenum p_data_format = GL_UNSIGNED_BYTE;

        if (p_format == GL_DEPTH_COMPONENT)
        {
            p_data_format = GL_FLOAT;
        }

        if (p_pixel_storage_mode != 4)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, p_pixel_storage_mode);
        }

        Create(p_texture);
        glTextureStorage2D(p_texture.id, 1, p_format, p_texture.width, p_texture.height);
        glTextureSubImage2D(p_texture.id, 0, 0, 0, p_texture.width, p_texture.height, GetTextureFormat(p_format), p_data_format, &p_bitmap);

        // Set texture parameters
        glTextureParameteri(p_texture.id, GL_TEXTURE_MIN_FILTER, p_filter_type);
        glTextureParameteri(p_texture.id, GL_TEXTURE_MAG_FILTER, p_filter_type);
        glTextureParameteri(p_texture.id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(p_texture.id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // If not default, reset to default
        if (p_pixel_storage_mode != 4)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        }
    }
}