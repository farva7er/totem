#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform vec4 fColor;
uniform sampler2D texSampler;

void main()
{
   FragColor = fColor * 
      texture(texSampler, TexCoords);
}
