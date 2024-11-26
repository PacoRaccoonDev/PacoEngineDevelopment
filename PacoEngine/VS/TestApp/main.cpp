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

struct BaseVertex
{
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec4 vertex_color;
    uint64_t  texture_handle;
};

int main(int argc, char* argv[])
{
    Window main_window;
    main_window.info = WindowInfo{ "PacoEngine" };
    main_window.rect = WindowRect{ 50 , 50 , 800 , 800 };

    if (!WINDOW_FUNCTIONS::INIT::InitializeRenderContext(main_window))
    {
        return -1;
    }

    Material testMaterial;

    {
        const char* fontVertexShaderSourcePath = "../Shader/shader_src/testshader.vert";
        const char* fontFragmentShaderSourcePath = "../Shader/shader_src/testshader.frag";
        ShaderFunctions::Compile(testMaterial.shader, fontVertexShaderSourcePath, fontFragmentShaderSourcePath);
        ShaderFunctions::Use(testMaterial.shader);
    }

    if (!glad_glGetTextureHandleARB || !glad_glMakeTextureHandleResidentARB) {
        std::cerr << "Bindless textures are not supported!" << std::endl;
        return 0;
    }


    const char* version = (const char*)glGetString(GL_VERSION);
    std::cout << "OpenGL Version: " << version << std::endl;

    Texture testTex;

    TextureFunctions::Create(testTex);
    TextureFunctions::GenerateTextureFromFile("uvt.png", testTex, true, true);

    Texture testTex2;

    TextureFunctions::Create(testTex2);
    TextureFunctions::GenerateTextureFromFile("uvt.png", testTex2, true, true);

    glUseProgram(testMaterial.shader.id);
    GLint p = glGetUniformLocation(testMaterial.shader.id, "a");
    GLint o = glGetUniformLocation(testMaterial.shader.id, "b");
    GLint l = glGetUniformLocation(testMaterial.shader.id, "c");

    glUniformHandleui64ARB(p, testTex2.handle);
    glUniformHandleui64ARB(o, testTex.handle);
    printerr(glGetError());

    InputManager main_input_manager;

    bool program_should_close = false;

    //INPUT_FUNCTIONS::InitializeConnectedControllers(main_input_manager);

    while (program_should_close == false)
    {
        {
            SDL_Event e;

            INPUT_FUNCTIONS::UpdateInputStatesPrePoll(main_input_manager);

            while (SDL_PollEvent(&e))
            {
                INPUT_FUNCTIONS::HandleControllerEvents(e, main_input_manager);
                INPUT_FUNCTIONS::UpdateInputEvent(e, main_input_manager);
                WINDOW_FUNCTIONS::UpdateWindowEvent(e, program_should_close, main_window);
            }

            INPUT_FUNCTIONS::UpdateInputStatesPostPoll(main_input_manager);


        }

        glClearColor(0.0, 0.0, 0.4, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        WINDOW_FUNCTIONS::Swap(main_window);
    }

    WINDOW_FUNCTIONS::Delete(main_window);

    return 0;
}