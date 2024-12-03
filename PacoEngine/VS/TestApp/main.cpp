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

    Texture fontAtlasTexture;
    TTFFontAtlas fontAtlas(128.0f, 1024, 1024, 96);

    FontAtlasFunctions::LoadTTFFontAtlasFromFile("arial.ttf", fontAtlas, fontAtlasTexture);

    float textOffsetX = 0;
    float textOffsetY = 0;

    TTFFontQuad fontQuad;
    fontQuad.character = '@';

    FontAtlasFunctions::InitializeFontQuadFromAtlas(fontAtlas, fontQuad, textOffsetX, textOffsetY);

    // Define a quad
    //BaseVertex vertices[] = {
    //    {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f} },
    //    {{ 0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },
    //    {{ 0.5f,  0.5f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f} },
    //    {{-0.5f,  0.5f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 0.0f, 1.0f} }
    //};

    //// Define a quad
    BaseVertex vertices[] = {
        {{-0.5f, -0.5f, 0.0f}, {fontQuad.quad.s0, fontQuad.quad.t1}, {1.0f, 0.0f, 0.0f, 1.0f} },
        {{ 0.5f, -0.5f, 0.0f}, {fontQuad.quad.s1, fontQuad.quad.t1}, {0.0f, 1.0f, 0.0f, 1.0f} },
        {{ 0.5f,  0.5f, 0.0f}, {fontQuad.quad.s1, fontQuad.quad.t0}, {0.0f, 0.0f, 1.0f, 1.0f} },
        {{-0.5f,  0.5f, 0.0f}, {fontQuad.quad.s0, fontQuad.quad.t0}, {1.0f, 1.0f, 0.0f, 1.0f} }
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
    VAOFunctions::SetAllAttributeFormatsByVertexAttributes(vao, vertices[0].attributes);

    // Link buffers to VAO
    VAOFunctions::LinkBuffers(vao, sizeof(BaseVertex), vbo, ebo);
    VAOFunctions::Unbind();


    Material testMaterial;

    {
        const char* fontVertexShaderSourcePath = "../Shader/shader_src/testshader.vert";
        const char* fontFragmentShaderSourcePath = "../Shader/shader_src/testshader.frag";
        ShaderFunctions::Compile(testMaterial.shader, fontVertexShaderSourcePath, fontFragmentShaderSourcePath);
        ShaderFunctions::Use(testMaterial.shader);
        testMaterial.textures.push_back(fontAtlasTexture);
        //TextureFunctions::GenerateTextureFromFile("uvt.png", testMaterial.textures[0], true);
    }

    if (!glad_glGetTextureHandleARB || !glad_glMakeTextureHandleResidentARB) {
        std::cerr << "Bindless textures are not supported!" << std::endl;
        return 0;
    }


    const char* version = (const char*)glGetString(GL_VERSION);
    std::cout << "OpenGL Version: " << version << std::endl;



    ShaderFunctions::Use(testMaterial.shader);

    printerr(glGetError());

    TextureFunctions::BindTextureUnit(testMaterial.textures[0], 0);
    //glBindTextureUnit(0, testTex.id);
    //glUniform1i(glGetUniformLocation(testMaterial.shader.id, "a"), 0);

    printerr(glGetError());

    InputManager main_input_manager;

    bool program_should_close = false;

    //INPUT_FUNCTIONS::InitializeConnectedControllers(main_input_manager);

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

    WindowFunctions::Delete(main_window);
    FontAtlasFunctions::Delete(fontAtlas);

    return 0;
}