#include <iostream>
#include <SDL2/SDL.h>
#include <glad/gl.h>
#include <PacoEngineLibrary/paco_engine_defines.h>
#include <PacoEngineLibrary/core/Logging.h>
#include <PacoEngineLibrary/core/Window.h>
#include <PacoEngineLibrary/core/Input.h>
#include <PacoEngineLibrary/core/Texture.h>
#include <PacoEngineLibrary/core/RendererBufferObjects.h>
#include <PacoEngineLibrary/core/Shader.h>
#include <PacoEngineLibrary/core/Material.h>
#include <PacoEngineLibrary/core/Font.h>

struct BaseVertex
{
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec4 vertex_color;

    static const std::vector<VertexAttribute> attributes;
};

// Define the static attribute descriptions
const std::vector<VertexAttribute> BaseVertex::attributes = {
    {0, 3, GL_FLOAT, GL_FALSE, offsetof(BaseVertex, position)},      // Position
    {1, 2, GL_FLOAT, GL_FALSE, offsetof(BaseVertex, uv)},            // UV
    {2, 4, GL_FLOAT, GL_FALSE, offsetof(BaseVertex, vertex_color)},  // Color
};

int main(int argc, char* argv[])
{
    Window main_window;
    main_window.info = WindowInfo{ "PacoEngine" };
    main_window.rect = WindowRect{ 50 , 50 , 800 , 800 };

    if (!WindowFunctions::INIT::InitializeRenderContext(main_window))
    {
        return -1;
    }

    //Alpha Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    std::string fontPath = "arial.ttf";

    std::ifstream file(fontPath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open font file: " + fontPath);
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw std::runtime_error("Failed to read font file: " + fontPath);
    }

    // Initialize stb_truetype
    stbtt_fontinfo font;
    if (!stbtt_InitFont(&font, buffer.data(), stbtt_GetFontOffsetForIndex(buffer.data(), 0))) {
        throw std::runtime_error("Failed to initialize font.");
    }

    // Set scale for desired font size
    float fontSize = 64.0f; // Desired font size in pixels
    float scale = stbtt_ScaleForPixelHeight(&font, fontSize);

    // Access a glyph by Unicode codepoint
    int unicodeCodepoint = 0x03A6; // Unicode for 'A'

    wchar_t unicodeChar = L'☺';

    std::string str = "ǋ";

    int glyphIndex = stbtt_FindGlyphIndex(&font, unicodeChar);

    if (glyphIndex == 0) {
        std::cout << "Glyph not found for Unicode codepoint: " << unicodeCodepoint << std::endl;
        return 1;
    }

    // Get glyph metrics
    int advanceWidth, leftSideBearing;
    stbtt_GetGlyphHMetrics(&font, glyphIndex, &advanceWidth, &leftSideBearing);
    std::cout << "Advance Width: " << advanceWidth * scale << " pixels\n";
    std::cout << "Left Side Bearing: " << leftSideBearing * scale << " pixels\n";

    // Get glyph bounding box
    int x0, y0, x1, y1;
    stbtt_GetGlyphBox(&font, glyphIndex, &x0, &y0, &x1, &y1);
    std::cout << "Glyph Bounding Box: (" << x0 * scale << ", " << y0 * scale << ") to ("
        << x1 * scale << ", " << y1 * scale << ")\n";

    // Render the glyph to a bitmap
    int width, height, xOffset, yOffset;
    unsigned char* bitmap = stbtt_GetGlyphBitmap(&font, scale, scale, glyphIndex, &width, &height, &xOffset, &yOffset);

    // Print bitmap as ASCII art (for demonstration)
    //std::cout << "Glyph Bitmap:\n";
    //for (int y = 0; y < height; ++y) {
    //    for (int x = 0; x < width; ++x) {
    //        std::cout << (bitmap[y * width + x] > 128 ? '#' : '.');
    //    }
    //    std::cout << '\n';
    //}

    Texture tex;
    tex.width = width;
    tex.height = height;
    TextureFunctions::GenerateTextureFromBitmap(tex,*bitmap, 1, GL_R8, GL_LINEAR);

    // Free the bitmap memory
    stbtt_FreeBitmap(bitmap, nullptr);

    BaseVertex vertices[] = {
        // Bottom-left corner
        {{-0.05f, -0.05f, 0.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f} },

        // Bottom-right corner
        {{ 0.05f, -0.05f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },

        // Top-right corner
        {{ 0.05f,  0.05f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f} },

        // Top-left corner
        {{-0.05f,  0.05f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f} }
    };

    //// Define a quad
    //BaseVertex vertices[] = {
    //    {{-0.05f, -0.05f, 0.0f}, {q->uv_bottom_left}, {1.0f, 0.0f, 0.0f, 1.0f} },
    //    {{ 0.05f, -0.05f, 0.0f}, {q->uv_bottom_right}, {0.0f, 1.0f, 0.0f, 1.0f} },
    //    {{ 0.05f,  0.05f, 0.0f}, {q->uv_top_right}, {0.0f, 0.0f, 1.0f, 1.0f} },
    //    {{-0.05f,  0.05f, 0.0f}, {q->uv_top_left}, {1.0f, 1.0f, 0.0f, 1.0f} }
    //};

    //// Define a quad
    //BaseVertex vertices[] = {
    //    {{(q->rect_top_left.x / 256.0f) * -0.5f - 0.5f, (q->rect_top_left.y / 256.0f) * 0.5f - 0.5f, 0.0f}, {q->uv_bottom_left}, {1.0f, 0.0f, 0.0f, 1.0f} },
    //    {{(q->rect_bottom_right.x / 256.0f) * 0.5f + 0.5f, (q->rect_top_left.y / 256.0f) * 0.5f - 0.5f, 0.0f}, {q->uv_bottom_right}, {0.0f, 1.0f, 0.0f, 1.0f} },
    //    {{ (q->rect_bottom_right.x / 256.0f) * 0.5f + 0.5f, (q->rect_bottom_right.y / 256.0f) * -0.5f + 0.5f, 0.0f}, {q->uv_top_right}, {0.0f, 0.0f, 1.0f, 1.0f} },
    //    {{(q->rect_top_left.x / 256.0f) * -0.5f - 0.5f, (q->rect_bottom_right.y / 256.0f) * -0.5f + 0.5f, 0.0f}, {q->uv_top_left}, {1.0f, 1.0f, 0.0f, 1.0f} }
    //};


    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    // Create VAO, VBO, EBO
    VAO vao;
    VBO vbo;
    EBO ebo;

    VAOFunctions::Create(vao);
    VBOFunctions::Create(vbo);
    EBOFunctions::Create(ebo);

    VAOFunctions::Bind(vao);
    VBOFunctions::PreallocateBufferWithData(vbo, 4, vertices, true);
    EBOFunctions::PreallocateBufferWithData(ebo, 6, indices, true);
    VAOFunctions::SetAllAttributeFormatsByVertexAttributes(vao, BaseVertex::attributes);

    // Link buffers to VAO
    VAOFunctions::LinkBuffers(vao, sizeof(BaseVertex), vbo, ebo);
    VAOFunctions::Unbind();


    Material testMaterial;

    {
        const char* fontVertexShaderSourcePath = "../Shader/shader_src/testshader.vert";
        const char* fontFragmentShaderSourcePath = "../Shader/shader_src/testshader.frag";
        ShaderFunctions::Compile(testMaterial.shader, fontVertexShaderSourcePath, fontFragmentShaderSourcePath);
        ShaderFunctions::Use(testMaterial.shader);
        testMaterial.textures.push_back(tex);
        Texture t;
        TextureFunctions::Create(t);
        TextureFunctions::GenerateTextureFromFile("uvt.png", t, true);
        testMaterial.textures.push_back(t);
    }

    const char* version = (const char*)glGetString(GL_VERSION);
    std::cout << "OpenGL Version: " << version << std::endl;



    ShaderFunctions::Use(testMaterial.shader);

    printerr(glGetError());

    TextureFunctions::BindTextureUnit(testMaterial.textures[0], 0);

    printerr(glGetError());

    InputManager main_input_manager;

    bool program_should_close = false;

    while (program_should_close == false)
    {
        {
            SDL_Event e;

            InputFunctions::UpdateInputStatesPrePoll(main_input_manager);

            while (SDL_PollEvent(&e))
            {
                InputFunctions::HandleControllerEvents(e, main_input_manager);
                InputFunctions::UpdateInputEvent(e, main_input_manager);
                WindowFunctions::UpdateWindowEvent(e, program_should_close, main_window);
            }

            InputFunctions::UpdateInputStatesPostPoll(main_input_manager);


        }

        glClearColor(0.0, 0.0, 0.4, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vao.id);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        WindowFunctions::Swap(main_window);
    }

    VAOFunctions::Delete(vao);
    VBOFunctions::Delete(vbo);
    EBOFunctions::Delete(ebo);
    WindowFunctions::Delete(main_window);

    return 0;
}