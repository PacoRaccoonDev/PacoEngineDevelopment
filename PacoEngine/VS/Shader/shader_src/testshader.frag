#version 450
#extension GL_ARB_bindless_texture : require

layout (location = 0) in vec2 TexCoord;
layout (location = 1) in vec4 VertColor;

  
layout(bindless_sampler) uniform sampler2D a;
layout(bindless_sampler) uniform sampler2D b;
layout(bindless_sampler) uniform sampler2D c; 

layout(location = 0) out vec4 FragColor;
  
void main()
{    
    vec4 asd = texture(b, TexCoord);
    vec4 asdf = texture(c, TexCoord);
    FragColor = texture(a, TexCoord);

}