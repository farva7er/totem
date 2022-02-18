#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D textSampler;
uniform vec4 fColor;

void main()
{
   vec2 flippedTexCoords = vec2(TexCoords.x, 1 - TexCoords.y);
   vec4 sampled = 
      vec4(1.0, 1.0, 1.0, texture(textSampler, flippedTexCoords).r);
   FragColor = fColor * sampled;
}
