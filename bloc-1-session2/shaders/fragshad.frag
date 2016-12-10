#version 330 core

out vec4 FragColor;
in  vec3 fcolor;
//EJERCICIO 1 HALF-LINE
// void main() {
//     FragColor = vec4(0, 0, 0, 1);
//     if (gl_FragCoord.x < 250.)
//         FragColor = vec4(1.,0.,0.,1.);
// }
//EJERCICIO 2 PIJAMA DE RALLAS
// void main() {
//     FragColor = vec4(0, 0, 0, 1);
//     if (mod (gl_FragCoord.x, 50) < 25)
//         FragColor = vec4(1.,0.,0.,1.);
// }


void main() {
    FragColor = vec4(fcolor, 1);
}