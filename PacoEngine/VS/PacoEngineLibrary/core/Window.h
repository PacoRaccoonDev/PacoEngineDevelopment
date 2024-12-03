#pragma once
#include <SDL2/SDL.h>
#include <glad/gl.h>
#include <string>

struct WindowInfo
{
    std::string title = "NO_NAME_GIVEN";

    bool window_mouse_focus = false;
    bool window_keyboard_focus = false;
    bool window_fullScreen = false;
    bool window_minimized = false;

    int current_screen_id = 0;

    Uint32 window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

};

struct WindowRect
{
    int x = 0;
    int y = 0;
    unsigned int width = 0;
    unsigned int height = 0;
};


struct Window
{
    SDL_GLContext sdl_window_context;
    SDL_Window* sdl_window_instance;
    WindowRect rect;
    WindowInfo info;
};

namespace WindowFunctions
{
    static bool CreateWindow(Window& p_window)
    {
        p_window.sdl_window_instance = SDL_CreateWindow(p_window.info.title.c_str(), p_window.rect.x, p_window.rect.y, p_window.rect.width, p_window.rect.height, p_window.info.window_flags);

        if (p_window.sdl_window_instance == nullptr)
        {
            std::cerr << "Failed to Create SDL Window Instance : " << SDL_GetError() << std::endl;
            return false;
        }

        p_window.sdl_window_context = SDL_GL_CreateContext(p_window.sdl_window_instance);

        if (p_window.sdl_window_context == nullptr)
        {
            std::cerr << "Failed to Create SDL Context : " << SDL_GetError() << std::endl;
            return false;
        }

        return true;

    }

    namespace INIT
    {   
        namespace CORE
        {
            static bool InitializeSDL(Uint32 p_initFlags = SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER)
            {
                if (SDL_Init(p_initFlags) < 0)
                {
                    std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
                    return false;
                }

                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

                //By Default V-Sync is off
                SDL_GL_SetSwapInterval(false);

                return true;

            }

            //BEFORE CALLING THIS YOU NEED A GL CONTEXT
            static bool InitializeGladGL()
            {
                if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress))
                {
                    std::cerr << "Failed to initialize GLAD" << std::endl;
                    SDL_Quit();
                    return false;
                }
                else
                {
                    return true;
                }
            }
        }

        static bool InitializeRenderContext(Window& p_window,Uint32 p_initFlags = SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER)
        {
            bool sdl_init;
            bool window_creation;
            bool glad_loaded_proc;

            sdl_init = WindowFunctions::INIT::CORE::InitializeSDL(p_initFlags);
            window_creation = WindowFunctions::CreateWindow(p_window);
            glad_loaded_proc = WindowFunctions::INIT::CORE::InitializeGladGL();

            if(!sdl_init || !window_creation || !glad_loaded_proc)
            {
                std::cerr << "Failed Render Context creation";
                return false;
            }else
            {
                return true;
            }
        }


    }

    static bool CreateWindowWithSharedContext(std::string p_title, Window& p_window, SDL_GLContext& p_shared_context)
    {
        p_window.sdl_window_instance = SDL_CreateWindow(p_title.c_str(), p_window.rect.x, p_window.rect.y, p_window.rect.width, p_window.rect.height, p_window.info.window_flags);

        if (p_window.sdl_window_instance = nullptr)
        {
            std::cerr << "Failed to Create SDL Window Instance : " << SDL_GetError() << std::endl;
            return false;
        }

        p_window.sdl_window_context = p_shared_context;

        return true;

    }

    static void Delete(Window& p_window)
    {
        SDL_GL_DeleteContext(p_window.sdl_window_context);
        SDL_DestroyWindow(p_window.sdl_window_instance);

    }

    static void MakeCurrent(Window& p_window)
    {
        SDL_GL_MakeCurrent(p_window.sdl_window_instance, p_window.sdl_window_context);

    }

    // V-SYNC -1 : Adaptive (might not be supported) | 0 : Off | 1 : On  |
    static void SetVSync(bool p_vsyncState)
    {
        SDL_GL_SetSwapInterval(p_vsyncState);
    }

    static void SetTitle(Window& p_window, const char* p_new_title)
    {       
        SDL_SetWindowTitle(p_window.sdl_window_instance, p_new_title);
    }

    static void SetSize(Window& p_window, unsigned int p_width, unsigned int p_height)
    {
        p_window.rect.width = p_width;
        p_window.rect.height = p_height;
        SDL_SetWindowSize(p_window.sdl_window_instance, p_width, p_height);
    }

    static void Swap(Window& p_windowInstance)
    {
        SDL_GL_SwapWindow(p_windowInstance.sdl_window_instance);
    }

    void UpdateWindowEvent(SDL_Event& p_event, bool& p_quitEvent, Window& p_window)
    {
        bool quit = p_event.type == SDL_QUIT;
        bool windowEvent = p_event.type == SDL_WINDOWEVENT;


        if (quit) { p_quitEvent = true; return; }


        if (windowEvent)
        {
            switch (p_event.window.event)
            {
            case SDL_WINDOWEVENT_SIZE_CHANGED:
            case SDL_WINDOWEVENT_RESIZED:
                p_window.rect.width = p_event.window.data1;
                p_window.rect.height = p_event.window.data2;
                break;
            case SDL_WINDOWEVENT_MOVED:
                SDL_GetWindowPosition(p_window.sdl_window_instance, &p_window.rect.x, &p_window.rect.y);
                break;
                //Happens on Mouse Release on new Window
            case SDL_WINDOWEVENT_DISPLAY_CHANGED:
                p_window.info.current_screen_id = SDL_GetWindowDisplayIndex(p_window.sdl_window_instance);
                break;
            case SDL_WINDOWEVENT_MINIMIZED:
                p_window.info.window_minimized = true;
                break;
            case SDL_WINDOWEVENT_MAXIMIZED:
                p_window.info.window_minimized = false;
                break;
            case SDL_WINDOWEVENT_RESTORED:
                p_window.info.window_minimized = false;
                break;
            case SDL_WINDOWEVENT_ENTER:
                p_window.info.window_mouse_focus = true;
                break;
            case SDL_WINDOWEVENT_LEAVE:
                p_window.info.window_mouse_focus = false;
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                p_window.info.window_keyboard_focus = true;
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                p_window.info.window_keyboard_focus = false;
                break;
            }
        }

    }

    static void HideCursor(bool p_value)
    {
        SDL_ShowCursor(!p_value);
    }

    static void LockCursorInWindowAndHide(bool p_value)
    {
        SDL_bool b = p_value ? SDL_TRUE : SDL_FALSE;
        SDL_SetRelativeMouseMode(b);
    }
}