#pragma once
#include <glad/gl.h>
#include <glm/glm.hpp>
#include "Mesh.h"
#include "RendererBufferObjects.h"

namespace RendererEnums
{
	enum DepthTestType : GLenum
	{
		DEPTH_TEST_NEVER = GL_NEVER,        // The fragment never passes the test.
		DEPTH_TEST_LESS = GL_LESS,          // The fragment passes if its depth value is less than the value in the depth buffer (closer to the camera).
		DEPTH_TEST_EQUAL = GL_EQUAL,        // The fragment passes if its depth value is equal to the depth buffer.
		DEPTH_TEST_LEQUAL = GL_LEQUAL,      // The fragment passes if its depth value is less than or equal to the depth buffer.
		DEPTH_TEST_GREATER = GL_GREATER,    // The fragment passes if its depth value is greater than the depth buffer (farther from the camera).
		DEPTH_TEST_NOTEQUAL = GL_NOTEQUAL,  // The fragment passes if its depth value is not equal to the depth buffer.
		DEPTH_TEST_GEQUAL = GL_GEQUAL,      // The fragment passes if its depth value is greater than or equal to the depth buffer.
		DEPTH_TEST_ALWAYS = GL_ALWAYS		 // The fragment always passes the test (ignores depth).
	};

	enum BlendFactor : GLenum
	{
		ZERO = GL_ZERO,											// (0, 0, 0, 0) - Multiplies the color by 0.
		ONE = GL_ONE,											// (1, 1, 1, 1) - Multiplies the color by 1 (the color remains unchanged).

		SRC_COLOR = GL_SRC_COLOR,								// Uses the source color (C_source).
		ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,			// Uses (1 - C_source).

		DST_COLOR = GL_DST_COLOR,								// Uses the destination color (C_destination).
		ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,			// Uses (1 - C_destination).

		SRC_ALPHA = GL_SRC_ALPHA,								// Uses the source alpha value (A_source).
		ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,			// Uses (1 - A_source).

		DST_ALPHA = GL_DST_ALPHA,								// Uses the destination alpha value (A_destination).
		ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,			// Uses (1 - A_destination).

		CONSTANT_COLOR = GL_CONSTANT_COLOR,						// Uses a constant color defined with glBlendColor().
		ONE_MINUS_CONSTANT_COLOR = GL_ONE_MINUS_CONSTANT_COLOR, // Uses (1 - constant color).

		CONSTANT_ALPHA = GL_CONSTANT_ALPHA,						// Uses a constant alpha defined with glBlendColor().
		ONE_MINUS_CONSTANT_ALPHA = GL_ONE_MINUS_CONSTANT_ALPHA, // Uses (1 - constant alpha).

		SRC_ALPHA_SATURATE = GL_SRC_ALPHA_SATURATE				// Uses the source alpha saturation (min(A_source, 1 - A_destination)).
	};

	enum FaceWindingOrder : GLenum
	{
		ClockWise = GL_CW,
		CounterClockWise = GL_CCW
	};

}

namespace RendererConfigFunctions
{
	static void SetDepthTestRange(double p_nearPlane, double p_farPlane)
	{
		glDepthRange(p_nearPlane, p_farPlane);
	}

	static void SetDepthTestFunction(GLenum p_testType = GL_LESS)
	{
		glDepthFunc(p_testType);
	}

	static void EnableDepthTest()
	{
		glEnable(GL_DEPTH_TEST);
	}

	static void DisableDepthTest()
	{
		glDisable(GL_DEPTH_TEST);
	}

	static void SetBlendFunction(GLenum p_sourceColorScale = GL_SRC_ALPHA, GLenum p_destinationColorScale = GL_ONE_MINUS_SRC_ALPHA)
	{
		glBlendFunc(p_sourceColorScale, p_destinationColorScale);
	}

	static void EnableBlend()
	{
		glEnable(GL_BLEND);
	}

	static void DisableBlend()
	{
		glDisable(GL_BLEND);
	}

	static void SetFaceWindingOrder(GLenum p_windingOrder = GL_CCW)
	{
		glFrontFace(p_windingOrder);
	}

	static void EnableFaceCull()
	{
		glEnable(GL_CULL_FACE);
	}

	static void DisableFaceCull()
	{
		glDisable(GL_CULL_FACE);
	}

}

namespace RendererDrawFunctions
{
	static void Clear(glm::vec4 p_color)
	{
		glClearColor(p_color.r, p_color.g, p_color.b, p_color.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	static void ClearWithDepth(glm::vec4 p_color)
	{
		glClearColor(p_color.r, p_color.g, p_color.b, p_color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}


	static void DrawElements(GLenum p_drawMode, unsigned int p_elementCount)
	{
		glDrawElements(p_drawMode, p_elementCount, GL_UNSIGNED_INT, 0);
	}

	/*static void DrawElementsIndirect(GLenum p_drawMode, DrawElementsIndirectCommand& p_cmd)
	{
		glDrawElementsIndirect(p_drawMode, GL_UNSIGNED_INT,0);
	}*/


	const char* getGLErrorStr(GLenum err) {
		switch (err) {
		case GL_NO_ERROR:
			return "No error";
		case GL_INVALID_ENUM:
			return "Invalid enum";
		case GL_INVALID_VALUE:
			return "Invalid value";
		case GL_INVALID_OPERATION:
			return "Invalid operation";
		case GL_STACK_OVERFLOW:
			return "Stack overflow";
		case GL_STACK_UNDERFLOW:
			return "Stack underflow";
		case GL_OUT_OF_MEMORY:
			return "Out of memory";
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			return "Invalid framebuffer operation";
		default:
			return "Unknown error";
		}
	}

	// Function to print all OpenGL errors
	void printGLErrors(const char* label) {
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			std::cerr << "OpenGL Error [" << label << "]: " << getGLErrorStr(err) << std::endl;
		}
	}

}