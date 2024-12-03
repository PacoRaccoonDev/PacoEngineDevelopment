#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "Texture.h"
#include <stb/stb_truetype.h>

struct TTFFontAtlas
{
public:
    int atlas_width = 512;
    int atlas_height = 512;
    int number_of_glyphs = 96;    // Number of glyphs to include (e.g. ASCII 32-127)
    float pixel_height = 64.0f;

    stbtt_bakedchar* baked_chars;

    // Allocates memory on the heap CLEAN IT WITH DELETE
    TTFFontAtlas(float p_pixel_height, int p_atlas_width, int p_atlas_height, int p_number_of_glyphs)
        : atlas_width(p_atlas_width), atlas_height(p_atlas_height), number_of_glyphs(p_number_of_glyphs), pixel_height(p_pixel_height)
    {
        baked_chars = new stbtt_bakedchar[number_of_glyphs];
    }
};

struct TTFFontQuad
{
    int character = 'A';
    float pos_x = 0;
    float pos_y = 0;
    stbtt_aligned_quad quad;
};

namespace FontAtlasFunctions
{
    static void Delete(TTFFontAtlas& p_atlas_to_free)
    {
        delete[] p_atlas_to_free.baked_chars;
    }

    static void InitializeFontQuadFromAtlas(TTFFontAtlas& p_font_atlas, TTFFontQuad& p_font_quad, float& p_x, float& p_y)
    {
        stbtt_GetBakedQuad(p_font_atlas.baked_chars, p_font_atlas.atlas_width, p_font_atlas.atlas_height, p_font_quad.character - 32, &p_x, &p_y, &p_font_quad.quad, 1);
    }

    static bool LoadTTFFontAtlasFromFile(const char* p_ttf_file_path, TTFFontAtlas& p_font_atlas, Texture& p_font_texture, GLenum p_filter_type = GL_LINEAR)
    {
        stbtt_fontinfo p_font;

        std::ifstream file(p_ttf_file_path, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open TTF file.");
            return false;
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<unsigned char> font_buffer(size);

        if (!file.read(reinterpret_cast<char*>(font_buffer.data()), size)) {
            throw std::runtime_error("Error reading TTF file.");
            return false;
        }

        if (!stbtt_InitFont(&p_font, font_buffer.data(), stbtt_GetFontOffsetForIndex(font_buffer.data(), 0))) {
            std::cerr << "Failed to initialize font.\n";
            return false;
        }

        unsigned char* atlas_bitmap = new unsigned char[p_font_atlas.atlas_width * p_font_atlas.atlas_height];

        stbtt_BakeFontBitmap(font_buffer.data(), 0, p_font_atlas.pixel_height, atlas_bitmap, p_font_atlas.atlas_width, p_font_atlas.atlas_height, 32, p_font_atlas.number_of_glyphs, p_font_atlas.baked_chars);

        p_font_texture.width = p_font_atlas.atlas_width;
        p_font_texture.height = p_font_atlas.atlas_height;

        TextureFunctions::Create(p_font_texture);
        TextureFunctions::GenerateTextureFromBitmap(p_font_texture, *atlas_bitmap, 1, GL_R8, p_filter_type);

        delete[] atlas_bitmap;
        return true;
    }
}
