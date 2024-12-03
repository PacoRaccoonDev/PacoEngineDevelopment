#pragma once
#include "Shader.h"
#include "Texture.h"

struct Material
{
	Shader shader;

	std::vector<Texture> textures;
};


//#include <unordered_map>
//#include <variant>
//#include <glm/glm.hpp>
//#include "Shader.h"
//#include "Texture.h"
//
//using UniformValue = std::variant<int, float, glm::vec2, glm::vec3, glm::vec4, glm::mat4>;
//
//struct Material
//{
//    Shader shader;
//    std::unordered_map<std::string, UniformValue> uniforms;
//    std::vector<Texture> textures;
//};
//
//namespace MaterialFunctions
//{
//
//    template<typename T>
//    static void SetUniform(Material& p_material, const std::string& p_name, const T& p_value) {
//        p_material.uniforms[p_name] = p_value;
//    }
//
//    static void ApplyUniforms(Material& p_material) {
//
//        ShaderFunctions::Use(p_material.shader.id);
//        for (const auto& [name, value] : p_material.uniforms) {
//            std::visit([&](auto&& arg) {
//                ShaderFunctions::SetUniform(p_material.shader.id, name, arg);
//                }, value);
//        }
//
//    }
//
//    void BindTextures(Material& p_material) {
//        for (unsigned i = 0; i < p_material.textures.size(); ++i) {
//            glActiveTexture(GL_TEXTURE0 + i);
//            glBindTexture(GL_TEXTURE_2D, p_material.textures[i].id);
//
//        }
//    }
//
//    void Use(Material& p_material) {
//        ApplyUniforms(p_material);
//        BindTextures(p_material);
//    }
//}