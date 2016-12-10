#version 330 core

in vec3 fmatamb;
in vec3 fmatdiff;
in vec3 fmatspec;
in float fmatshin;

vec3 colFocus = vec3(0.8, 0.8, 0.8);
vec3 llumAmbient = vec3(0.2, 0.2, 0.2);

uniform vec3 scoLlum;

in vec3 vecNorm;
in vec4 vertexSCO;

out vec4 FragColor;

vec3 Lambert (vec3 NormSCO, vec3 L) 
{
    vec3 colRes = llumAmbient * fmatamb;
    if (dot (L, NormSCO) > 0)
      colRes = colRes + colFocus * fmatdiff * dot (L, NormSCO);
    return (colRes);
}

vec3 Phong (vec3 NormSCO, vec3 L, vec4 vertSCO) 
{
    vec3 colRes = Lambert (NormSCO, L);
    if (dot(NormSCO,L) < 0)
      return colRes;

    vec3 R = reflect(-L, NormSCO);
    vec3 V = normalize(-vertSCO.xyz);

    if ((dot(R, V) < 0) || (fmatshin == 0))
      return colRes;
    float shine = pow(max(0.0, dot(R, V)), fmatshin);
    return (colRes + fmatspec * colFocus * shine); 
}

void main()
{
    vec3 L = normalize(scoLlum - vec3 (vertexSCO));
    FragColor = vec4(Phong(vecNorm, L, vertexSCO), 1);
}
