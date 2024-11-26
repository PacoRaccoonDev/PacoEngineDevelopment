#pragma once
//#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//struct UIVertex
//{
//	glm::vec3 position;
//	glm::vec2 uv;
//	glm::vec4 vertex_color;
//	float texture_handle;
//};


//
//struct Vertex {
//    glm::vec3 position;
//    glm::vec2 uv;
//};
//
//template<typename VertexType>
//struct Mesh {
//    std::vector<VertexType> vertices;
//    std::vector<unsigned int> indices;
//};
//
//const Mesh<Vertex> BASEMESH_QUAD = {
//    // Vertices of the quad
//    {
//        { glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f) }, // Bottom-left
//        { glm::vec3(-1.0f, 1.0f, 0.0f) , glm::vec2(0.0f, 0.0f) }, // Bottom-right
//        { glm::vec3(1.0f,  1.0f, 0.0f) , glm::vec2(1.0f, 0.0f) }, // Top-right
//        { glm::vec3(1.0f,  -1.0f, 0.0f), glm::vec2(1.0f, 1.0f) }  // Top-left
//    },
//    // Indices for the two triangles of the quad
//    {
//        0, 1, 2,  // First triangle
//        2, 3, 0   // Second triangle
//    }
//};
//
//namespace MeshFunctions
//{
//
//}