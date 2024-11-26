#pragma once
#include <glad/gl.h>
#include <iostream>

void printerr(GLenum error)
{
    if (error != GL_NO_ERROR) {
        switch (error) {
        case GL_INVALID_ENUM:
            std::cerr << "OpenGL Error: GL_INVALID_ENUM\n";
            break;
        case GL_INVALID_VALUE:
            std::cerr << "OpenGL Error: GL_INVALID_VALUE\n";
            break;
        case GL_INVALID_OPERATION:
            std::cerr << "OpenGL Error: GL_INVALID_OPERATION\n";
            break;
        case GL_STACK_OVERFLOW:
            std::cerr << "OpenGL Error: GL_STACK_OVERFLOW\n";
            break;
        case GL_STACK_UNDERFLOW:
            std::cerr << "OpenGL Error: GL_STACK_UNDERFLOW\n";
            break;
        case GL_OUT_OF_MEMORY:
            std::cerr << "OpenGL Error: GL_OUT_OF_MEMORY\n";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            std::cerr << "OpenGL Error: GL_INVALID_FRAMEBUFFER_OPERATION\n";
            break;
        default:
            std::cerr << "OpenGL Error: Unknown error code\n";
        }
    }
}