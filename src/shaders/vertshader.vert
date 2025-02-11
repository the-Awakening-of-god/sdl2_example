#version 410 core 

layout (location = 0) in vec3 position;

out vec4 vertextColor;

void main()
{ 
     gl_Position = vec4(position, 1.0);
     vertextColor = vec4(0.5f, 0.5f, 0.2f, 1.0f); 
}
