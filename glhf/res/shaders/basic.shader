#SHADER VERTEX
#version 330 core

layout(location = 0) in vec4 position;

void main()
{
   gl_Position = position;
}

#SHADER FRAGMENT
#version 330 core

layout(location = 0) out vec4 color;

void main()
{
   color = vec4(0.96f, 0.75f, 0.25f, 1.0f);
}
