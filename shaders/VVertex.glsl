#version 410 core
//first element(index) in the VBO
layout (location=0) in vec3 position;
// third attribute the UV values from our VAO
layout (location=1)in vec3 inColour;
// we use this to pass the UV values to the frag shader
uniform mat4 MVP;
out vec3 PlantColor;
void main()
{
    //TODO:if to decide which part will be draw, for example, if the number of points less than l-system part, so the gl_Position will draw this part, if more, it will draw the object model

    gl_Position = MVP *vec4(position,1.0);
    PlantColor = inColour;
}