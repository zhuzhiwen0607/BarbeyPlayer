#version 330 core
layout (location = 0) in vec2 ipos;
layout (location = 1) in vec2 itex;
out vec4 otex;

void main()
{
    gl_Position = vec4(ipos.x, ipos.y, 0.0, 1.0);
    otex = vec4(itex.x, itex.y, 0.0, 1.0);
}
