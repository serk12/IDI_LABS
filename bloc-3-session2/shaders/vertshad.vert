#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;

uniform mat3 NormalMatrix;
uniform vec3 scoLlumV;

// vec3 colFocus = vec3(0.8, 0.8, 0.8);
// vec3 llumAmbient = vec3(0.2, 0.2, 0.2);
// vec3 posFocus = vec3(1, 1, 1);

out vec3 fmatamb;
out vec3 fmatdiff;
out vec3 fmatspec;
out float fmatshin;
out vec3 scoLlum;
out vec3 vecNorm;
out vec4 vertexSCO;

void main()
{
    fmatamb = matamb;
    fmatdiff = matdiff;
    fmatspec = matspec;
    fmatshin = matshin;
    scoLlum = scoLlumV;
    vecNorm = normalize(NormalMatrix * normal);
    vertexSCO = view * TG * vec4 (vertex, 1.0);
    gl_Position = proj  * vertexSCO;
}


// vecVertexLlum = normalize(scoLlum - vec3 (view * TG * vec4 (vertex, 1.0)));
    