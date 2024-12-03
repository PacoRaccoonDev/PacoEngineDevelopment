#pragma once
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <vector>
#include "Texture.h"
#include <variant>

struct DrawElementsIndirectCommand
{
	GLuint  count;			// Number of indices
	GLuint  instance_count;	// Number of instances
	GLuint  first_index;	// Offset in the index buffer
	GLuint  base_vertex;	// Base vertex offset
	GLuint  base_instance;	// Must Be 0
};

struct VertexAttribute {
	GLuint index;          // The attribute location in the vertex shader (0, 1, 2, etc.)
	GLint size;            // The number of components (1, 2, 3, 4, etc.)
	GLenum type;           // The OpenGL data type (e.g., GL_FLOAT, GL_INT)
	GLboolean normalized;  // Whether the attribute should be normalized (GL_TRUE/GL_FALSE)
	GLuint offset;         // Offset of the attribute in the struct (in bytes)
};

/*Add static const std::vector<VertexAttribute> attributes; to each VertexDataType struct
  Defining the static member:
	const std::vector<VertexAttribute> VertexData::attributes = {
	{0, 3, GL_FLOAT, GL_FALSE, offsetof(VertexData, position)},
	{1, 3, GL_FLOAT, GL_FALSE, offsetof(VertexData, normal)},
	{2, 2, GL_FLOAT, GL_FALSE, offsetof(VertexData, texCoord)}
};*/

struct VBO
{
	GLuint id;
};

namespace VBOFunctions
{
	static void Create(VBO& p_vbo) {
		glCreateBuffers(1, &p_vbo.id);
	}

	template<typename TVertexType>
	static void PreallocateBufferMemory(VBO& p_vbo, unsigned int p_vertex_count, bool p_is_static = false) {

		GLenum flags = GL_DYNAMIC_STORAGE_BIT;

		if(p_is_static)
		{
			flags = 0;
		}

		GLsizeiptr size = sizeof(TVertexType) * p_vertex_count;

		glNamedBufferStorage(p_vbo.id, size, nullptr, flags);
	}

	template<typename TVertexType>
	static void UpdateBufferData(VBO& p_vbo, unsigned int p_vertex_count, const void* p_data, GLintptr p_offset = 0) {

		GLsizeiptr size = sizeof(TVertexType) * p_vertex_count;

		glNamedBufferSubData(p_vbo.id, p_offset, size, p_data);
	}

	template<typename TVertexType>
	void PreallocateBufferWithData(VBO& p_vbo, unsigned int p_vertex_count, TVertexType* p_data, bool p_is_static = false) {
		
		GLenum flags = GL_DYNAMIC_STORAGE_BIT;

		if (p_is_static)
		{
			flags = 0;
		}

		GLsizeiptr size = sizeof(TVertexType) * p_vertex_count;

		glNamedBufferStorage(p_vbo.id, size, p_data, flags);
	}

	void PreallocateBufferMemory(VBO& p_vbo, GLsizeiptr p_size, bool p_is_static = false) {

		GLenum flags = GL_DYNAMIC_STORAGE_BIT;

		if (p_is_static)
		{
			flags = 0;
		}

		glNamedBufferStorage(p_vbo.id, p_size, nullptr, flags);
	}

	void UpdateBufferData(VBO& p_vbo, const void* p_data, GLsizeiptr p_size, GLintptr p_offset = 0) {
		glNamedBufferSubData(p_vbo.id, p_offset, p_size, p_data);
	}

	void PreallocateBufferWithData(VBO& p_vbo, GLsizeiptr p_size, const float* p_data, bool p_is_static = false) {

		GLenum flags = GL_DYNAMIC_STORAGE_BIT;

		if (p_is_static)
		{
			flags = 0;
		}

		glNamedBufferStorage(p_vbo.id, p_size, p_data, flags);
	}


	void Delete(VBO& p_vbo) {
		glDeleteBuffers(1, &p_vbo.id);
		p_vbo.id = 0;
	}


}

struct EBO
{
	GLuint id;
};

namespace EBOFunctions{

	static void Create(EBO& p_ebo) {
		glCreateBuffers(1, &p_ebo.id); // DSA: Create the EBO
	}

	static void Bind(EBO& p_ebo) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_ebo.id);
	}

	static void Unbind(){
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}


	static void PreallocateBufferMemory(EBO& p_ebo, unsigned int p_index_count, bool p_is_static = false) {

		GLenum flags = GL_DYNAMIC_STORAGE_BIT;

		if (p_is_static)
		{
			flags = 0;
		}

		GLsizeiptr size = sizeof(unsigned int) * p_index_count;

		glNamedBufferStorage(p_ebo.id, size, nullptr, flags);
	}

	static void UpdateBufferData(EBO& p_ebo, unsigned int p_index_count, const void* p_data, GLintptr p_offset = 0) {

		GLsizeiptr size = sizeof(unsigned int) * p_index_count;

		glNamedBufferSubData(p_ebo.id, p_offset, size, p_data);
	}

	static void PreallocateBufferWithData(EBO& p_ebo, unsigned int p_index_count, const unsigned int* p_data, bool p_is_static = false)
	{
		GLenum flags = GL_DYNAMIC_STORAGE_BIT;

		if (p_is_static)
		{
			flags = 0;
		}

		GLsizeiptr size = sizeof(unsigned int) * p_index_count;

		glNamedBufferStorage(p_ebo.id, size, p_data, flags);
	}

	static void Delete(EBO& p_ebo)
	{
		glDeleteBuffers(1, &p_ebo.id);
	}
}

struct VAO
{
	GLuint id;
};

namespace VAOFunctions
{
	static void Create(VAO& p_vao)
	{
		glCreateVertexArrays(1, &p_vao.id);
	}

	static void Bind(VAO& p_vao) {
		glBindVertexArray(p_vao.id);
	}

	static void Unbind() {
		glBindVertexArray(0);
	}

	template<typename TVertexType>
	static void LinkBuffers(VAO& p_vao, unsigned int p_vertex_count, VBO& p_vbo, EBO& p_ebo)
	{
		GLsizeiptr size = sizeof(TVertexType) * p_vertex_count;

		glVertexArrayVertexBuffer(p_vao.id, 0, p_vbo.id, 0, size);
		glVertexArrayElementBuffer(p_vao.id, p_ebo.id);
	}

	static void SetAttributeFormat(VAO& p_vao, GLuint p_attrib_index, GLuint p_binding_index, GLuint p_number_of_fields, GLenum p_attribute_type, GLuint p_field_offset)
	{
		glEnableVertexArrayAttrib(p_vao.id, p_attrib_index);
		glVertexArrayAttribBinding(p_vao.id, p_attrib_index, p_binding_index);
		glVertexArrayAttribFormat(p_vao.id, p_attrib_index, p_number_of_fields, p_attribute_type, GL_FALSE, p_field_offset);
	}

	static void SetAllAttributeFormatsByVertexAttributes(VAO& p_vao, std::vector<VertexAttribute> p_attributes )
	{
		for (const VertexAttribute& attribute : p_attributes) {

			SetAttributeFormat(p_vao,
				attribute.index,              
				0,                            //(set to 0 if using a single binding)
				attribute.size,               
				attribute.type,               
				attribute.offset);
		}
	}

	static void LinkBuffers(VAO& p_vao,GLsizeiptr p_vertex_size_in_bytes, VBO& p_vbo, EBO& p_ebo)
	{
		glVertexArrayVertexBuffer(p_vao.id, 0, p_vbo.id, 0, p_vertex_size_in_bytes);
		glVertexArrayElementBuffer(p_vao.id, p_ebo.id);
	}

	static void Delete(VAO& p_vao)
	{
		glDeleteVertexArrays(1, &p_vao.id);
	}

}

//Example of types you can use as uniforms
//using UniformValue = std::variant<int, float, glm::vec2, glm::vec3, glm::vec4, glm::mat4, uint64_t>;

struct UBO
{
	GLuint id;
	GLuint binding_point;
};

namespace UBOFunctions {

	static void Create(UBO& p_ubo)
	{
		glCreateBuffers(1, &p_ubo.id);
	}

	static void Bind(UBO& p_ubo)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, p_ubo.id);
	}

	static void Unbind(UBO& p_ubo)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	template<typename TUniformBuffer>
	static void AllocateBufferMemory(UBO& p_ubo, bool p_is_static = false)
	{
		GLenum flags = GL_DYNAMIC_DRAW;

		if (p_is_static)
		{
			flags = GL_STATIC_DRAW;
		}

		glNamedBufferData(p_ubo.id, sizeof(TUniformBuffer), nullptr, flags);
	}

	static void AllocateBufferMemory(UBO& p_ubo, const size_t p_size_in_bytes, bool p_is_static = false)
	{
		GLenum flags = GL_DYNAMIC_DRAW;

		if (p_is_static)
		{
			flags = GL_STATIC_DRAW;
		}

		glNamedBufferData(p_ubo.id, p_size_in_bytes, nullptr, flags);
	}


	template<typename TUniformBuffer>
	static void UpdateBufferData(UBO& p_ubo, const void* p_data) {
		glNamedBufferSubData(p_ubo.id, 0, sizeof(TUniformBuffer), p_data);
	}

	static void UpdateBufferData(UBO& p_ubo, const size_t p_size_in_bytes, const void* p_data) {
		glNamedBufferSubData(p_ubo.id, 0, p_size_in_bytes, p_data);
	}

	template<typename TUniformBuffer>
	static void AllocateBufferMemoryWithData(UBO& p_ubo,const void* p_data, bool p_is_static) {

		GLenum flags = GL_DYNAMIC_DRAW;

		if (p_is_static)
		{
			flags = GL_STATIC_DRAW;
		}

		glNamedBufferData(p_ubo.id, sizeof(TUniformBuffer), p_data, flags);
	}

	static void AllocateBufferMemoryWithData(UBO& p_ubo, const size_t p_size_in_bytes, const void* p_data, bool p_is_static) {

		GLenum flags = GL_DYNAMIC_DRAW;

		if (p_is_static)
		{
			flags = GL_STATIC_DRAW;
		}

		glNamedBufferData(p_ubo.id, p_size_in_bytes, p_data, flags);
	}

	static void BindBufferBase(UBO& p_ubo) {

		glBindBufferBase(GL_UNIFORM_BUFFER, p_ubo.binding_point, p_ubo.id);
	}

	static void BindBufferRange(UBO& p_ubo, size_t p_offest, size_t p_size)
	{
		glBindBufferRange(GL_UNIFORM_BUFFER, p_ubo.binding_point, p_ubo.id, p_offest, p_size);
	}

	static void Delete(UBO& p_ubo)
	{
		glDeleteBuffers(1, &p_ubo.id);
	}
}

struct IBO
{
	GLuint id;
};

namespace IBOFunctions
{
	
	static void Create(IBO& p_ibo) {
		glCreateBuffers(1, &p_ibo.id);
	}

	static void Bind(IBO& p_ibo)
	{
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, p_ibo.id);
	}

	static void AllocateBufferMemory(IBO& p_ibo, unsigned int p_command_count, bool p_is_static = false)
	{
		GLenum flags = GL_DYNAMIC_STORAGE_BIT;

		if (p_is_static)
		{
			flags = 0;
		}

		glNamedBufferStorage(p_ibo.id, sizeof(DrawElementsIndirectCommand) * p_command_count, nullptr, GL_DYNAMIC_STORAGE_BIT);
	}

	static void UpdateBufferData(IBO& p_ibo, const DrawElementsIndirectCommand* p_cmd, unsigned int p_command_count)
	{
		glNamedBufferSubData(p_ibo.id, 0, sizeof(DrawElementsIndirectCommand) * p_command_count, &p_cmd);
	}

	static void AllocateBufferMemoryWithData(IBO& p_ibo, GLsizeiptr p_commands_size, const DrawElementsIndirectCommand* p_commands, bool p_is_static)
	{
		GLenum flags = GL_DYNAMIC_STORAGE_BIT;

		if (p_is_static)
		{
			flags = 0;
		}

		glNamedBufferStorage(p_ibo.id, p_commands_size, p_commands, flags);
	}

	static void Delete(IBO& p_ibo) {
		glDeleteBuffers(1, &p_ibo.id);	
		p_ibo.id = 0;
	}
}

struct RBO
{
	GLuint id;
};

namespace RBOFunctions
{

	void Create(RBO& p_rbo,int p_width, int p_height)
	{
		glCreateRenderbuffers(1, &p_rbo.id);
		glNamedRenderbufferStorage(p_rbo.id, GL_DEPTH24_STENCIL8, p_width, p_height);
	}

	void Attach(RBO& p_rbo,GLuint p_framebuffer_id)
	{
		glNamedFramebufferRenderbuffer(p_framebuffer_id, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, p_rbo.id);
	}

	void Delete(RBO& p_rbo)
	{
		glDeleteRenderbuffers(1, &p_rbo.id);
	}
}

struct FBO
{
	GLuint id;
	Texture texture;
	RBO rbo;
};

namespace FBOFunctions 
{

	void Create(FBO& p_fbo,int p_width, int p_height)
	{
		// Create the framebuffer
		glCreateFramebuffers(1, &p_fbo.id);

		// Create the texture
		p_fbo.texture.width = p_width;
		p_fbo.texture.height = p_height;
		TextureFunctions::Create(p_fbo.texture);
		glTextureStorage2D(p_fbo.texture.id, 1, GL_RGB8, p_fbo.texture.width, p_fbo.texture.height);

		glNamedFramebufferTexture(p_fbo.id, GL_COLOR_ATTACHMENT0, p_fbo.texture.id, 0);

		// Create and attach the renderbuffer object (RBO)
		RBOFunctions::Create(p_fbo.rbo,p_width, p_height);
		RBOFunctions::Attach(p_fbo.rbo, p_fbo.id);

		// Check if the framebuffer is complete
		if (glCheckNamedFramebufferStatus(p_fbo.id, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		}
	}

	void Bind(FBO& p_fbo)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, p_fbo.id);
	}

	void Unbind(FBO& p_fbo)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Delete(FBO& p_fbo)
	{
		glDeleteFramebuffers(1, &p_fbo.id);
		TextureFunctions::Delete(p_fbo.texture);
		RBOFunctions::Delete(p_fbo.rbo);
	}
}

struct SamplerObject
{
	GLuint id;
};

namespace SamplerObjectFunctions
{
	// Create a sampler object
	static void Create(SamplerObject& p_sampler)
	{
		glCreateSamplers(1, &p_sampler.id);
	}

	// Set filtering parameters
	static void SetFiltering(SamplerObject& p_sampler, GLenum p_min_filter, GLenum p_mag_filter)
	{
		glSamplerParameteri(p_sampler.id, GL_TEXTURE_MIN_FILTER, p_min_filter);
		glSamplerParameteri(p_sampler.id, GL_TEXTURE_MAG_FILTER, p_mag_filter);
	}

	// Set wrap parameters
	static void SetWrapMode(SamplerObject& p_sampler, GLenum p_coord_handle_type, GLenum p_wrap_type/*, GLenum p_wrap_t, GLenum p_wrap_r*/)
	{
		glSamplerParameteri(p_sampler.id, p_coord_handle_type, p_wrap_type);     // Defines the wrapping mode for the S (horizontal) coordinate.
		//glSamplerParameteri(p_sampler.id, GL_TEXTURE_WRAP_T, p_wrap_t); // Defines the wrapping mode for the T (vertical) coordinate.
		//glSamplerParameteri(p_sampler.id, GL_TEXTURE_WRAP_R, p_wrap_r); // Defines the wrapping mode for the R (depth) coordinate in the case of 3D texture
	}

	// Set border color for the sampler
	static void SetBorderColor(SamplerObject& p_sampler, const GLfloat p_border_color[4])
	{
		glSamplerParameterfv(p_sampler.id, GL_TEXTURE_BORDER_COLOR, p_border_color);
	}

	// Set anisotropic filtering
	static void SetAnisotropy(SamplerObject& p_sampler, GLfloat p_max_anisotropy)
	{
		GLfloat maxSupportedAnisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxSupportedAnisotropy);

		if (p_max_anisotropy > maxSupportedAnisotropy)
		{
			p_max_anisotropy = maxSupportedAnisotropy;
		}

		glSamplerParameterf(p_sampler.id, GL_TEXTURE_MAX_ANISOTROPY, p_max_anisotropy);
	}

	// Bind the sampler to a texture unit
	static void Bind(SamplerObject& p_sampler, GLuint p_texture_unit)
	{
		glBindSampler(p_texture_unit, p_sampler.id);
	}

	// Unbind the sampler from a texture unit
	static void Unbind(GLuint p_texture_unit)
	{
		glBindSampler(p_texture_unit, 0);
	}

	// Delete the sampler object
	static void Delete(SamplerObject& p_sampler)
	{
		glDeleteSamplers(1, &p_sampler.id);
		p_sampler.id = 0;
	}
}