//std
#include <iostream>
#include <vector>

//vendor
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <glad/glad/gl.h>


//RenderBuffer Objects
struct VertexAttribute {
    GLuint index;          // The attribute location in the vertex shader (0, 1, 2, etc.)
    GLint size;            // The number of components (1, 2, 3, 4, etc.)
    GLenum type;           // The OpenGL data type (e.g., GL_FLOAT, GL_INT)
    GLboolean normalized;  // Whether the attribute should be normalized (GL_TRUE/GL_FALSE)
    GLuint offset;         // Offset of the attribute in the struct (in bytes)
};


struct RenderComponent
{
    GLuint vbo;
    GLuint ebo;
    GLuint vao;
};

namespace RenderComponentFunctions
{
    static void Create(RenderComponent& p_renderComponent)
    {
        glCreateBuffers(1, &p_renderComponent.vbo);
        glCreateBuffers(1, &p_renderComponent.ebo);
        glCreateVertexArrays(1, &p_renderComponent.vao);
    }

    static void Bind(RenderComponent& p_renderComponent)
    {
        glBindVertexArray(p_renderComponent.vao);
    }

    static void Unbind() 
    {
        glBindVertexArray(0);
    }

    template<typename TVertexType>
    static void PreallocateBuffersMemory(RenderComponent& p_renderComponent, unsigned int p_vertexCount, unsigned int  p_indexCount, bool p_isStatic = false)
    {
        GLenum flags = GL_DYNAMIC_STORAGE_BIT;

        if (p_isStatic)
        {
            flags = 0;
        }

        GLsizeiptr verticesSize = sizeof(TVertexType) * p_vertexCount;

        GLsizeiptr indicesSize = sizeof(unsigned int) * p_indexCount;

        glNamedBufferStorage(p_renderComponent.vbo, vertexSize, nullptr, flags);
        glNamedBufferStorage(p_renderComponent.ebo, size, nullptr, flags);

    }

    template<typename TVertexType>
    static void UpdateBuffersData(RenderComponent& p_renderComponent, const void* p_vertexData, const void* p_indexData, unsigned int p_vertexCount, unsigned int p_indexCount, GLintptr p_offset = 0) {

        GLsizeiptr vertexSize = sizeof(TVertexType) * p_vertexCount;
        GLsizeiptr indexSize = sizeof(unsigned int) * p_indexCount;

        glNamedBufferSubData(p_renderComponent.vbo, p_offset, size, p_data);
        glNamedBufferSubData(p_renderComponent.ebo, p_offset, size, p_data);
    }

    template<typename TVertexType>
    static void UpdateVertexBufferData(RenderComponent& p_renderComponent, const void* p_data, unsigned int p_vertexCount, GLintptr p_offset = 0) {

        GLsizeiptr size = sizeof(TVertexType) * p_vertexCount;

        glNamedBufferSubData(p_renderComponent.vbo, p_offset, size, p_data);
    }

    static void UpdateIndexBufferData(RenderComponent& p_renderComponent, const void* p_data, unsigned int p_indexCount, GLintptr p_offset = 0) {

        GLsizeiptr size = sizeof(unsigned int) * p_indexCount;

        glNamedBufferSubData(p_renderComponent.ebo, p_offset, size, p_data);
    }

    static void SetAttributeFormats(RenderComponent& p_renderComponent, std::vector<VertexAttribute> p_attributes)
    {
        GLuint vao = p_renderComponent.vao;

        for (const VertexAttribute& attribute : p_attributes) {

            glEnableVertexArrayAttrib(vao, attribute.index);
            glVertexArrayAttribBinding(vao, attribute.index, 0);
            glVertexArrayAttribFormat(vao, attribute.index, attribute.size, attribute.type, GL_FALSE, attribute.offset);

        }
    }

    template<typename TVertexType>
    static void LinkBuffers(RenderComponent& p_renderComponent, unsigned int p_vertexCount)
    {
        GLuint vao = p_renderComponent.vao;

        GLsizeiptr size = sizeof(TVertexType);

        glVertexArrayVertexBuffer(vao, 0, p_renderComponent.vbo, 0, size);
        glVertexArrayElementBuffer(vao, p_renderComponent.ebo);
    }

    static void Delete(RenderComponent& p_renderComponent)
    {
        glDeleteBuffers(1, &p_renderComponent.vbo);
        glDeleteBuffers(1, &p_renderComponent.ebo);
        glDeleteVertexArrays(1, &p_renderComponent.vao);

    }

}

/*
*     VBOFunctions::PreallocateBufferMemory(vbo, sizeof(BaseVertex) * 4 * numberOfCells );
    VBOFunctions::UpdateBufferData<BaseVertex>(vbo,vertices, 4);
    VBOFunctions::UpdateBufferData<BaseVertex>(vbo,vertices2, 4, sizeof(BaseVertex)*4);
    EBOFunctions::PreallocateBufferMemory(ebo, 6  * sizeof(float));
    EBOFunctions::UpdateBufferData(ebo, indices, 6);
    for (auto& var : indices)
    {
        var += 4;
    }
    EBOFunctions::UpdateBufferData(ebo, indices, 6, sizeof(GLuint) * 6);
    //VBOFunctions::PreallocateBufferWithData(vbo, 4 * numberOfCells, vertices, true);
    //EBOFunctions::PreallocateBufferWithData(ebo, 6 * numberOfCells, indices, true);
    VAOFunctions::SetAllAttributeFormatsByVertexAttributes(vao, BaseVertex::attributes);

    // Link buffers to VAO
    VAOFunctions::LinkBuffers(vao, sizeof(BaseVertex), vbo, ebo);
    VAOFunctions::Unbind();
*/


int main(int argc, char** argv)
{
    if (!SDL_SetAppMetadata("Example PacoEngine App", "1.0", NULL)) 
    {
        SDL_Log("Error on SDL_SetAppMetadata : %s", SDL_GetError());
        return -1;
    }

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD)) 
    {
        SDL_Log("Error on SDL_Init : %s", SDL_GetError());
        return -1;
    }

    SDL_Window* window = nullptr;
    window = SDL_CreateWindow("Paco Engine", 800, 600, SDL_WINDOW_OPENGL);

    if(window == nullptr)
    {
        SDL_Log("Error on SDL_CreateWindow : %s", SDL_GetError());
        return -1;
    }
    SDL_Log("Window Allocated Remeber To Destroy!");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    //By Default V-Sync is off
    SDL_GL_SetSwapInterval(false);

    SDL_GLContext sdlGlCtx = nullptr;

    sdlGlCtx = SDL_GL_CreateContext(window);

    if (sdlGlCtx == nullptr)
    {
        SDL_Log("Failed to Create SDL Context : s%", SDL_GetError());
        return -1;
    }

    SDL_Log("GL Context Allocated Remeber To Destroy!");

    if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress))
    {
        SDL_Log("Failed to initialize GLAD");
        SDL_Quit();
        return false;
    }

    Uint64 lastStep = SDL_GetTicks();
   
    bool windowShouldClose = false;
    SDL_Event sdlEvent;
    
    while(!windowShouldClose)
    {
        while(SDL_PollEvent(&sdlEvent))
        {
            switch (sdlEvent.window.type)
            {
            case SDL_EVENT_QUIT:
                windowShouldClose = true;
                SDL_Log("Quit Event");
                break;
            default:
                break;
            }
        }

        glClearColor(0.0, 0.0, 0.4, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        SDL_GL_SwapWindow(window);

    }


    SDL_DestroyWindow(window);
    SDL_GL_DestroyContext(sdlGlCtx);
    SDL_Quit();
    return 0;
}


