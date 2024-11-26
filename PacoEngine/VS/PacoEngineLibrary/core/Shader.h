#pragma once

//#include "Renderer.h"
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>

struct Shader
{
	GLuint id;
};

// Function to load GLSL shader source code from a file
static std::string LoadGLSLSource(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		return "";
	}

	std::string sourceCode((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());

	file.close();
	return sourceCode;
}
namespace ShaderFunctions
{
	static void Use(Shader& p_shader )
	{
		glUseProgram(p_shader.id);
		// RendererFunctions::printGLErrors("Error IN asdfsadgsahahadfhadhfad Stage");
	}

	// Function to compile a shader program using GLSL source code
	static void Compile(Shader& p_shader, const char* p_vertexSourcePath, const char* p_fragmentSourcePath)
	{
		// Load GLSL source code
		std::string vertexShaderSource = LoadGLSLSource(p_vertexSourcePath);
		std::string fragmentShaderSource = LoadGLSLSource(p_fragmentSourcePath);

		if (vertexShaderSource.empty() || fragmentShaderSource.empty()) {
			std::cerr << "Failed to load shader sources." << std::endl;
			return;
		}

		// Create and compile vertex shader from GLSL
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		const char* vertexSource = vertexShaderSource.c_str();
		glShaderSource(vertexShader, 1, &vertexSource, nullptr);
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			std::cout << "Vertex Shader Compile Error \n";
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			std::cerr << &infoLog[0] << std::endl;

			glDeleteShader(vertexShader);
			return;
		}

		// Create and compile fragment shader from GLSL
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		const char* fragmentSource = fragmentShaderSource.c_str();
		glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			std::cout << "Fragment Shader Compile Error \n";
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			std::cerr << &infoLog[0] << std::endl;

			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);
			return;
		}

		// Create shader program and link shaders
		p_shader.id = glCreateProgram();
		glAttachShader(p_shader.id, vertexShader);
		glAttachShader(p_shader.id, fragmentShader);
		glLinkProgram(p_shader.id);

		GLint isLinked = 0;
		glGetProgramiv(p_shader.id, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			std::cout << "Program Linking Error \n";
			GLint maxLength = 0;
			glGetProgramiv(p_shader.id, GL_INFO_LOG_LENGTH, &maxLength);

			if (maxLength > 0)
			{
				std::vector<GLchar> infoLog(maxLength);
				glGetProgramInfoLog(p_shader.id, maxLength, &maxLength, &infoLog[0]);

				std::cerr << &infoLog[0] << std::endl;
			}

			glDeleteProgram(p_shader.id);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			return;
		}

		// Clean up shaders as they are no longer needed
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
}
/*struct Shader
{
	GLuint id;
};

namespace ShaderFunctions
{

	static std::vector<uint32_t> LoadSPIRV(const std::string& filename)
	{
		// Open the file with the cursor at the end
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			std::cerr << "Failed to open file: " << filename << std::endl;
			return {};
		}

		// Determine the size of the file
		size_t file_size = (size_t)file.tellg();
		std::vector<uint32_t> buffer(file_size / sizeof(uint32_t));

		// Go back to the beginning of the file
		file.seekg(0);

		// Read the entire file into the buffer
		file.read(reinterpret_cast<char*>(buffer.data()), file_size);

		// Check for read failure
		if (!file) {
			std::cerr << "Failed to read the SPIR-V file: " << filename << std::endl;
			return {};
		}

		// Close the file
		file.close();

		return buffer;
	}

	static void Compile(GLuint& p_shader, const char* p_vertex_source_path, const char* p_fragment_source_path)
	{
		auto vertex_shader_spirv = LoadSPIRV(p_vertex_source_path);
		auto fragment_shader_spirv = LoadSPIRV(p_fragment_source_path);

		// Create and compile vertex shader from SPIR-V
		GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

		glShaderBinary(1, &vertex_shader, GL_SHADER_BINARY_FORMAT_SPIR_V, vertex_shader_spirv.data(), vertex_shader_spirv.size() * sizeof(uint32_t));

		glSpecializeShader(vertex_shader, "main", 0, nullptr, nullptr);
		GLint is_compiled = 0;
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &is_compiled);
		if (is_compiled == GL_FALSE)
		{
			std::cout << "Vert Compile Error \n";
			GLint max_length = 0;
			glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &max_length);

			// The max_length includes the NULL character
			std::vector<GLchar> info_log(max_length);
			glGetShaderInfoLog(vertex_shader, max_length, &max_length, &info_log[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertex_shader);

			return;
		}

		GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderBinary(1, &fragment_shader, GL_SHADER_BINARY_FORMAT_SPIR_V, fragment_shader_spirv.data(), fragment_shader_spirv.size() * sizeof(uint32_t));

		glSpecializeShader(fragment_shader, "main", 0, nullptr, nullptr);

		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &is_compiled);
		if (is_compiled == GL_FALSE)
		{
			std::cout << "Frag Compile Error \n";
			GLint max_length = 0;
			glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &max_length);

			// The max_length includes the NULL character
			std::vector<GLchar> info_log(max_length);
			glGetShaderInfoLog(fragment_shader, max_length, &max_length, &info_log[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragment_shader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertex_shader);
			return;
		}

		p_shader = glCreateProgram();
		glAttachShader(p_shader, vertex_shader);
		glAttachShader(p_shader, fragment_shader);
		glLinkProgram(p_shader);
		GLint is_linked = 0;

		glGetProgramiv(p_shader, GL_LINK_STATUS, (int*)&is_linked);
		if (is_linked == GL_FALSE)
		{
			// TO DO logging
			std::cout << "Linking Error \n";
			GLint max_length = 0;
			glGetProgramiv(p_shader, GL_INFO_LOG_LENGTH, &max_length);

			if (max_length > 0)
			{
				std::vector<GLchar> info_log(max_length);
				glGetProgramInfoLog(p_shader, max_length, &max_length, &info_log[0]);

				// We don't need the program anymore.
				glDeleteProgram(p_shader);
				// Don't leak shaders either.
				glDeleteShader(vertex_shader);
				glDeleteShader(fragment_shader);
			}

			return;
		}

		// RendererFunctions::printGLErrors("Error IN Compiling Stage");

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
	}

	static void Use(GLuint p_shader_id)
	{
		glUseProgram(p_shader_id);
		// RendererFunctions::printGLErrors("Error IN asdfsadgsahahadfhadhfad Stage");
	}

	// Template method for setting uniforms
	template<typename T>
	static void SetUniform(GLuint p_shader_id, const std::string& name, const T& value) {
		// TO DO logging
		std::cout << "This type is not supported as a uniform" << std::endl;
	}

	template<>
	static void SetUniform<int>(GLuint p_shader_id, const std::string& name, const int& value) {
		glUniform1i(glGetUniformLocation(p_shader_id, name.c_str()), value);
	}

	template<>
	static void SetUniform<float>(GLuint p_shader_id, const std::string& name, const float& value) {
		glUniform1f(glGetUniformLocation(p_shader_id, name.c_str()), value);
	}

	template<>
	static void SetUniform<glm::vec2>(GLuint p_shader_id, const std::string& name, const glm::vec2& value) {
		glUniform2fv(glGetUniformLocation(p_shader_id, name.c_str()), 1, &value[0]);
	}

	template<>
	static void SetUniform<glm::vec3>(GLuint p_shader_id, const std::string& name, const glm::vec3& value) {
		glUniform3fv(glGetUniformLocation(p_shader_id, name.c_str()), 1, &value[0]);
	}

	template<>
	static void SetUniform<glm::vec4>(GLuint p_shader_id, const std::string& name, const glm::vec4& value) {
		glUniform4fv(glGetUniformLocation(p_shader_id, name.c_str()), 1, &value[0]);
	}

	template<>
	static void SetUniform<glm::mat4>(GLuint p_shader_id, const std::string& name, const glm::mat4& value) {
		glUniformMatrix4fv(glGetUniformLocation(p_shader_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	// Specialization for GLuint64 (bindless texture handle)
	template<>
	static void SetUniform<GLuint64>(GLuint p_shader_id, const std::string& name, const GLuint64& value) {
		GLint location = glGetUniformLocation(p_shader_id, name.c_str());
		if (location == -1) {
			std::cerr << "Uniform '" << name << "' not found in shader program ID: " << p_shader_id << std::endl;
			return;
		}
		glUniformHandleui64ARB(location, value);
	}

	static void Delete(GLuint p_shader_id)
	{
		glDeleteProgram(p_shader_id);
	};
}*/
