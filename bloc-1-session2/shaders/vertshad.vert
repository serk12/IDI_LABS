#version 330 core

in vec3 color;
in vec3 vertex;

out vec3 fcolor;
void main()  {
//EJERCICIO 3 REDUCE AT HALF
//gl_Position = vec4 (0.5* vertex, 1.0);
    fcolor = color;
    gl_Position = vec4(vertex, 1.0);
}
