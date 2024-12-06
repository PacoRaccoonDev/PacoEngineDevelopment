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

    //Texture fontAtlasTexture;
    TTFFontAtlas font_atlas(256.0f, 2048, 2048, 96);
    FontAtlasFunctions::LoadTTFFontAtlasFromFile("arial.ttf", font_atlas);

    TTFFontQuad* q = &font_atlas.baked_quads[FontAtlasFunctions::GetQuadIndicesFromString(font_atlas,"@")[0]];

    //// Define a quad
    BaseVertex vertices[] = {
        {{-0.5f, -0.5f, 0.0f}, {q->uv_bottom_left}, {1.0f, 0.0f, 0.0f, 1.0f} },
        {{ 0.5f, -0.5f, 0.0f}, {q->uv_bottom_right}, {0.0f, 1.0f, 0.0f, 1.0f} },
        {{ 0.5f,  0.5f, 0.0f}, {q->uv_top_right}, {0.0f, 0.0f, 1.0f, 1.0f} },
        {{-0.5f,  0.5f, 0.0f}, {q->uv_top_left}, {1.0f, 1.0f, 0.0f, 1.0f} }
    };


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
        testMaterial.textures.push_back(font_atlas.texture);
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
    FontAtlasFunctions::Delete(font_atlas);
    WindowFunctions::Delete(main_window);

    return 0;
}