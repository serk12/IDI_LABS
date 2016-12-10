#version 330 core

in vec3 vertex;
in vec3 color;

uniform mat4 TG;
uniform mat4 proj;//EJERCICIO 1 add matrix of projection
uniform mat4 view;//EJERCICIO 2 add matrix of pos cam
out vec3 fcolor;

void main()  {
    fcolor = color;
    gl_Position = proj * view * TG * vec4 (vertex, 1.0);
}
    