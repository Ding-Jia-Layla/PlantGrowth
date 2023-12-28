#version 410 core
layout (location=0) out vec4 fragColour;
in vec3 PlantColor;

void main()
{
    fragColour.rgb = PlantColor;

}


