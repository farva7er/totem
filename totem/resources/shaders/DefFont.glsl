##vertex shader
#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTexCoords;

out vec2 TexCoords;
uniform mat4 vModelMat;
uniform mat4 vProjMat;

void main()
{
   gl_Position = vProjMat * vModelMat * vec4(vPos, 1.0f);
   TexCoords = vTexCoords;
}

##fragment shader
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

