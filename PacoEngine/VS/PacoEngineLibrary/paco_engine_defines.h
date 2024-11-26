#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define STB_TRUETYPE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define main SDL_main
#define GLM_ENABLE_EXPERIMENTAL

#include <SDL2/SDL.h>
#include <iostream>
#include <glad/gl.h>


/*

[SCRIPTING RULES]

struct/class names camel case = ThisIsAClassName;

snake case for variables = this_is_a_variable_name;

camel case for functions = ThisIsAFunction();

function parameter variables snake p_ case rule = p_this_is_a_parameter_variable;


[MEMORY LAYOUT AND FOOTPRINT]

structs should try to not be greater than 64 bytes for cacheline , for non critical structures aka not something i do heavy computation on every frame can exceed this

CACHELINE : 64    bytes
	   L1 : 32   kbytes
	   L2 : 256  kbytes
	   L3 : 2048 kbytes

bool		Boolean value (true or false)			1 byte
char		Character type							1 byte
wchar_t		Wide character type						2 or 4 bytes
short		Short integer							2 bytes
int			Integer									4 bytes
Uint32		unsignedInteger							4 bytes
long		Long integer							4 or 8 bytes
long long	Long long integer						8 bytes
float		Floating point							4 bytes
double		Double precision floating point			8 bytes
long double	Extended precision floating point		8, 12, or 16 bytes
void*		Pointer (size depends on architecture)  8 bytes (64-bit)
std::string	String (complex type)					Implementation-defined

*/