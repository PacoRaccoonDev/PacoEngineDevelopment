#version 460
layout(location = 0) in vec3 iPos;       // Position attribute
layout(location = 1) in vec2 iTexCoord;  // TexCoord attribute
layout(location = 2) in vec4 iVertColor;
layout(location = 3) in uvec2 iTexHandle;
  
layout (location = 0) out vec2 oTexCoord;
layout (location = 1) out vec4 oVertColor;
layout (location = 2) flat out uvec2 oTexHandle;
  
//layout (binding = 0) uniform UBO {
//    mat4 u_MVP;                        // Model-view-projection matrix, now as a uniform block
//};

void main()
{
    gl_Position = vec4(iPos, 1.0);  // Set output position
    oTexCoord = iTexCoord;           // Pass through TexCoord
    oTexHandle = iTexHandle;
}