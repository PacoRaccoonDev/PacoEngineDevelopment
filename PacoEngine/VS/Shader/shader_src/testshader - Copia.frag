#version 460

layout (location = 0) in vec2 TexCoord;
layout (location = 1) in vec4 VertColor;
layout (location = 2) flat in uvec2 TexHandle;
  
layout(binding = 0) uniform sampler2D texture1;  // Font texture sampler

layout(location = 0) out vec4 FragColor;
  
void main()
{
    // Reconstruct the 64-bit handle
    uint64_t handle = uint64_t(TexHandle.y) << 32 | uint64_t(TexHandle.x);

    // Convert the handle to a sampler2D
    vec4 smpl =  texture(sampler2D(handle),TexCoord);

    //vec4 discol = texture(smpl, TexCoord);  // Sample from texture
    FragColor = smpl;
    //FragColor = mix(discol,VertColor,0.5);

}