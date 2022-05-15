#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D textSampler;
uniform vec4 fColor;

void main()
{
   vec2 flippedTexCoords = vec2(TexCoords.x, 1 - TexCoords.y);
   float alphaSample = texture(textSampler, flippedTexCoords).r;
   vec4 sampled = vec4(1.0, 1.0, 1.0, alphaSample);
   FragColor = fColor * sampled;


   /* sdf - shader
   vec2 flippedTexCoords = vec2(TexCoords.x, 1 - TexCoords.y);
   float sd = texture(textSampler, flippedTexCoords).r;
   if(sd < 0.5f)
      discard;
   FragColor = fColor;
   */
}
