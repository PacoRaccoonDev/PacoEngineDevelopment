//#pragma once
//#include "GenericComponents.h"
//#include <glad/gl.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtx/rotate_vector.hpp>
//#include <glm/gtx/vector_angle.hpp>
//#include <glm/gtc/quaternion.hpp>
//#include <glm/gtx/quaternion.hpp>
//
//enum class CameraProjection {
//    ORTHOGRAPHIC,
//    PERSPECTIVE
//};
//
//struct Camera
//{
//public:
//    CameraProjection projectionType;
//
//    //position should be a component of the entity
//    //glm::vec3 position;
//
//    float rotX; // Pitch and Orthographic rotation
//    float rotY; // Yaw
//
//    float zoom;
//    glm::vec2 aspectRatio;
//
//    glm::vec3 front;
//    glm::vec3 right;
//    glm::vec3 up;
//    glm::vec3 worldUp;
//
//    float nearPlane;
//    float farPlane;
//
//    float fov;
//
//    glm::mat4 viewMatrix;
//    glm::mat4 projectionMatrix;
//};
//
//namespace CameraFunctions
//{
//    static glm::mat4 GetProjectionMatrix(Camera& p_cam)
//    {
//        if (p_cam.projectionType == CameraProjection::ORTHOGRAPHIC)
//        {
//            float zoomAmount = 1 + p_cam.zoom;
//            float top = p_cam.aspectRatio.y / zoomAmount;
//            float bottom = -p_cam.aspectRatio.y / zoomAmount;
//            float right = p_cam.aspectRatio.x / zoomAmount;
//            float left = -p_cam.aspectRatio.x / zoomAmount;
//
//            return glm::ortho(left, right, bottom, top, p_cam.nearPlane, p_cam.farPlane);
//        }
//        else
//        {
//            float zoomAmount = 1 + p_cam.zoom;
//            float endFov = p_cam.fov / zoomAmount;
//            return glm::perspective(glm::radians(endFov), p_cam.aspectRatio.x / p_cam.aspectRatio.y, p_cam.nearPlane, p_cam.farPlane);
//        }
//    }
//
//    static glm::mat4 GetViewMatrix(Camera& p_cam, Position& p_pos)
//    {
//        if (p_cam.projectionType == CameraProjection::ORTHOGRAPHIC)
//        {
//            glm::mat4 transform = glm::translate(glm::mat4(1.0f), p_pos)
//                * glm::rotate(glm::mat4(1.0f), glm::radians(p_cam.rotX), glm::vec3(0, 0, 1));
//            return glm::inverse(transform);
//        }
//        else
//        {
//            return glm::lookAt(
//                p_pos, // Camera position
//                p_pos + p_cam.front, // Look at point
//                p_cam.up  // Up vector
//            );
//        }
//    }
//
//    static void UpdateCameraVectors(Camera& p_cam)
//    {
//        glm::quat pitch = glm::angleAxis(glm::radians(p_cam.rotX), glm::vec3(1, 0, 0));
//        glm::quat yaw = glm::angleAxis(glm::radians(-p_cam.rotY), glm::vec3(0, 1, 0));
//
//        glm::quat orientation = yaw * pitch;
//        glm::mat4 rotationMatrix = glm::toMat4(orientation);
//
//        p_cam.front = glm::vec3(rotationMatrix * glm::vec4(0, 0, -1, 1));
//        p_cam.right = glm::vec3(rotationMatrix * glm::vec4(1, 0, 0, 1));
//        p_cam.up = glm::vec3(rotationMatrix * glm::vec4(0, 1, 0, 1));
//
//        p_cam.front = glm::normalize(p_cam.front);
//        p_cam.right = glm::normalize(p_cam.right);
//        p_cam.up = glm::normalize(p_cam.up);
//    }
//
//    static void UpdateCameraViewProjection(Camera& p_cam, Position& p_pos)
//    {
//        p_cam.projectionMatrix = CameraFunctions::GetProjectionMatrix(p_cam);
//        p_cam.viewMatrix = CameraFunctions::GetViewMatrix(p_cam, p_pos);
//    }
//
//    static glm::mat4 GetMVP(Camera& p_cam, Position& p_modelPos, float p_modelRotation = 0.0f, const glm::vec3& p_modelScale = glm::vec3(1.0f, 1.0f, 1.0f))
//    {
//        glm::mat4 model = glm::translate(glm::mat4(1.0f), p_modelPos)
//            * glm::rotate(glm::mat4(1.0f), glm::radians(p_modelRotation), glm::vec3(0.0f, 1.0f, 0.0f))
//            * glm::scale(glm::mat4(1.0f), p_modelScale);
//        glm::mat4 mvp = p_cam.projectionMatrix * p_cam.viewMatrix * model;
//        return mvp;
//    }
//
//    static glm::mat4 GetMVP(Camera& p_cam, Position& p_modelPos, const glm::quat& p_modelRotation, const glm::vec3& p_modelScale = glm::vec3(1.0f, 1.0f, 1.0f))
//    {
//        glm::mat4 model = glm::translate(glm::mat4(1.0f), p_modelPos)
//            * glm::toMat4(p_modelRotation)
//            * glm::scale(glm::mat4(1.0f), p_modelScale);
//        glm::mat4 mvp = p_cam.projectionMatrix * p_cam.viewMatrix * model;
//        return mvp;
//    }
//}