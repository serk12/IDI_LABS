#version 330 core

in vec3 color;
in vec3 vertex;
uniform float val; //make mov
uniform mat4 TG; //make mov
out vec3 fcolor;
void main()  {
    fcolor = color;
    gl_Position = TG * vec4(vertex*val, 1.0); //make mov
}
