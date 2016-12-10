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

uniform mat3 NormalMatrix;  //ex 5
uniform vec3 scoLlum; //ex 5

vec3 colFocus = vec3(0.8, 0.8, 0.8);
vec3 llumAmbient = vec3(0.2, 0.2, 0.2);
vec3 posFocus = vec3(1, 1, 1);

out vec3 fcolor;

vec3 Lambert (vec3 NormSCO, vec3 L) 
{
    vec3 colRes = llumAmbient * matamb;
    if (dot (L, NormSCO) > 0)
      colRes = colRes + colFocus * matdiff * dot (L, NormSCO);
    return (colRes);
}

vec3 Phong (vec3 NormSCO, vec3 L, vec4 vertSCO) 
{
    vec3 colRes = Lambert (NormSCO, L);
    if (dot(NormSCO,L) < 0)
      return colRes;

    vec3 R = reflect(-L, NormSCO);
    vec3 V = normalize(-vertSCO.xyz);

    if ((dot(R, V) < 0) || (matshin == 0))
      return colRes;
    float shine = pow(max(0.0, dot(R, V)), matshin);
    return (colRes + matspec * colFocus * shine); 
}

void main()
{	
    gl_Position = proj * view * TG * vec4 (vertex, 1.0);
    //     mat3 NormalMatrix = inverse (transpose (mat3 (view*TG))); //NormalMatdix que transforma en es sistema SCO // ex 1 / 2
    vec3 vecNorm = normalize(NormalMatrix * normal);  //ex 1 / 2
    //     vec3 scoLlum = vec3 (view * vec4 (1,1,1,1)); //ex 1 / 2
    vec3 vecVertexLlum = normalize(scoLlum - vec3 (view * TG * vec4 (vertex, 1.0)));//ex 1 / 2
    vec4 vertexSCO = view * TG * vec4 (vertex, 1.0); //ex 1 / 2
    fcolor = Phong(vecNorm, vecVertexLlum, vertexSCO); //ex 1 / 2
}
