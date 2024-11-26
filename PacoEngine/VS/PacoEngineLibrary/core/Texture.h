#pragma once
//#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <iostream>
#include <glad/gl.h>

struct Texture
{
    GLuint id;
    GLuint64 handle; // Handle for bindless textures
    int width, height;
};

namespace TextureFunctions
{
    static void Create(Texture& p_texture)
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &p_texture.id);
    }

    static void BindTextureUnit(Texture& p_texture, GLenum p_texture_unit) {

        if (p_texture_unit < GL_TEXTURE0 || p_texture_unit > GL_TEXTURE31) {
            std::cerr << "Invalid texture unit: " << p_texture_unit << std::endl;
            return;  // or handle the error
        }

        if (p_texture.id == 0) {
            std::cerr << "Invalid texture ID" << std::endl;
            return;  // or handle the error
        }

        glBindTextureUnit(p_texture_unit, p_texture.id);
    }

    static void MakeBindless(Texture& p_texture)
    {
        p_texture.handle = glGetTextureHandleARB(p_texture.id);
        glMakeTextureHandleResidentARB(p_texture.handle);

        //if (glMakeTextureHandleResidentARB) // Ensure the function is available
        //{

        //}
        //else
        //{
        //    std::cerr << "Bindless texture extension is not supported on this system!" << std::endl;
        //    p_texture.handle = 0;
        //}
    }

    static void MakeNonResident(Texture& p_texture)
    {
        if (glMakeTextureHandleNonResidentARB && p_texture.handle)
        {
            glMakeTextureHandleNonResidentARB(p_texture.handle);
            p_texture.handle = 0; // Invalidate handle
        }
    }

    static void Delete(Texture& p_texture)
    {
        MakeNonResident(p_texture); // Ensure the handle is non-resident before deleting
        glDeleteTextures(1, &p_texture.id);
    }

    static GLenum GetTextureFormat(GLenum& p_internal_format) {
        switch (p_internal_format) {
        case GL_RGBA8:
            return GL_RGBA;
            break;
        case GL_RGB8:
            return GL_RGB;
            break;
        case GL_R8:
            return GL_RED;
            break;
        case GL_DEPTH_COMPONENT:
            return GL_DEPTH_COMPONENT;
            break;
        default:
            fprintf(stderr, "Unsupported internal format: %u\n", p_internal_format);
            return 0; // Invalid format
            break;
        }
    }

    static void GenerateTextureFromFile(const char* p_path, Texture& p_texture, bool p_nearest_filtering = true, bool p_make_bindless = false)
    {
        // TO DO: Implement MipMapLevels

        auto p_filter_type = GL_NEAREST;

        if (!p_nearest_filtering)
        {
            p_filter_type = GL_LINEAR;
        }

        // Load Image Data
        int p_channels;
        unsigned char* p_img = stbi_load(p_path, &p_texture.width, &p_texture.height, &p_channels, 0);

        if (p_img == NULL) {
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

        if (p_make_bindless)
        {
            MakeBindless(p_texture);
        }
    }

    // PixelStorageMode: 1 = for fonts | 4 = default

    static void GenerateTextureFromBitmap(Texture& p_texture, unsigned char& p_bitmap, int p_pixel_storage_mode, GLenum p_format, GLenum p_filter_type = GL_LINEAR, bool p_make_bindless = false)
    {
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

        if (p_make_bindless)
        {
            MakeBindless(p_texture);
        }
    }
}
