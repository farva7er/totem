#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D textSampler;
uniform vec4 fColor;

void main()
{
   vec2 flippedTexCoords = vec2(TexCoords.x, 1 - TexCoords.y);
   float sd = texture(textSampler, flippedTexCoords).r;
   vec4 sampled = vec4(1.0, 1.0, 1.0, sd);
   FragColor = fColor * sampled;
   /*
   vec2 flippedTexCoords = vec2(TexCoords.x, 1 - TexCoords.y);
   float sd = texture(textSampler, flippedTexCoords).r - 128.0f;
   sd = (sd / 128.0f) * 2;
   vec4 sampled = vec4(1.0, 1.0, 1.0, sd);
   FragColor = fColor * sampled;
   */
}
