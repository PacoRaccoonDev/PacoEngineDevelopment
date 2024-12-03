#version 450

layout (location = 0) in vec2 TexCoord;
layout (location = 1) in vec4 VertColor;

  
layout(binding = 0) uniform sampler2D a;

layout(location = 0) out vec4 FragColor;
  
void main()
{    
    
     FragColor = texture(a, TexCoord);

}