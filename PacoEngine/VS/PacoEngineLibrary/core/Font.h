#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include "Texture.h"
#include <stb/stb_truetype.h>

struct TTFFontQuad
{
    int character = 'A';

    glm::vec2 rect_top_left;
    glm::vec2 rect_bottom_right;

    glm::vec2 uv_bottom_left;
    glm::vec2 uv_top_left;
    glm::vec2 uv_bottom_right;
    glm::vec2 uv_top_right;

};

struct TTFFontAtlas
{
public:
    int atlas_width = 512;
    int atlas_height = 512;
    int number_of_glyphs = 96;    // Number of glyphs to include (e.g. ASCII 32-127)
    float pixel_height = 64.0f;

    Texture texture;

    TTFFontQuad* baked_quads;

    // Allocates memory on the heap CLEAN IT WITH DELETE
    TTFFontAtlas(float p_pixel_height, int p_atlas_width, int p_atlas_height, int p_number_of_glyphs)
        : atlas_width(p_atlas_width), atlas_height(p_atlas_height), number_of_glyphs(p_number_of_glyphs), pixel_height(p_pixel_height)
    {
        //baked_chars = new stbtt_bakedchar[number_of_glyphs];
        baked_quads = new TTFFontQuad[number_of_glyphs];
    }
};

namespace FontAtlasFunctions
{
    static void Delete(TTFFontAtlas& p_atlas_to_free)
    {
        delete[] p_atlas_to_free.baked_quads;
    }

    static void InitializeFontQuadFromAtlas(TTFFontAtlas& p_font_atlas,const stbtt_bakedchar* p_baked_chars, TTFFontQuad& p_font_quad)
    {
        //Cursor Postion For Now Im Not Using It
        float pos_x = 0;
        float pos_y = 0;

        stbtt_aligned_quad quad_data;
        stbtt_GetBakedQuad(p_baked_chars, p_font_atlas.atlas_width, p_font_atlas.atlas_height, p_font_quad.character - 32, &pos_x, &pos_y, &quad_data, 1);
        p_font_quad.rect_top_left = glm::vec2(quad_data.x0, quad_data.y0);
        p_font_quad.rect_bottom_right = glm::vec2(quad_data.x1, quad_data.y1);

        //Correct Uv Order
        // s0 t1 | s1 t1 | s1 t0 | s0 t0 |
        p_font_quad.uv_bottom_left = glm::vec2(quad_data.s0, quad_data.t1);
        p_font_quad.uv_bottom_right = glm::vec2(quad_data.s1, quad_data.t1);
        p_font_quad.uv_top_right = glm::vec2(quad_data.s1, quad_data.t0);
        p_font_quad.uv_top_left = glm::vec2(quad_data.s0, quad_data.t0);
            
    }

    static bool LoadTTFFontAtlasFromFile(const char* p_ttf_file_path, TTFFontAtlas& p_font_atlas, GLenum p_filter_type = GL_LINEAR)
    {
        stbtt_fontinfo p_font;
        stbtt_bakedchar* baked_chars = new stbtt_bakedchar[p_font_atlas.number_of_glyphs];

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

        stbtt_BakeFontBitmap(font_buffer.data(), 0, p_font_atlas.pixel_height, atlas_bitmap, p_font_atlas.atlas_width, p_font_atlas.atlas_height, 32, p_font_atlas.number_of_glyphs, baked_chars);

        p_font_atlas.texture.width = p_font_atlas.atlas_width;
        p_font_atlas.texture.height = p_font_atlas.atlas_height;

        TextureFunctions::Create(p_font_atlas.texture);
        TextureFunctions::GenerateTextureFromBitmap(p_font_atlas.texture, *atlas_bitmap, 1, GL_R8, p_filter_type);

        //Initialize Font Quads 
        for (int i = 0; i < p_font_atlas.number_of_glyphs; ++i)
        {
            p_font_atlas.baked_quads[i].character = i + 32;
            InitializeFontQuadFromAtlas(p_font_atlas, baked_chars, p_font_atlas.baked_quads[i]);
        }

        delete[] atlas_bitmap;
        delete[] baked_chars;
        return true;
    }

    static std::vector<int> GetQuadIndicesFromString(const TTFFontAtlas& p_font_atlas, const std::string& p_text)
    {
        std::vector<int> indices;

        for (char c : p_text)
        {
            if (c >= 32 && c < 32 + p_font_atlas.number_of_glyphs) 
            {
                indices.push_back(c - 32);
            }
        }

        return indices;
    }
}
