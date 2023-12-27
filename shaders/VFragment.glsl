#version 410 core
layout (location=0) out vec4 fragColour;
in vec3 PlantColor;

void main()
{
    fragColour=vec4(PlantColor,1.0);

}