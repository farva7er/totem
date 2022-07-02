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

uniform vec4 fColor;
uniform sampler2D texSampler;

void main()
{
   FragColor = fColor * 
      texture(texSampler, TexCoords);
}
