#version 450 core
/**
 * \file params_quad_spacing_even.tese
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */
layout(quads, fractional_even_spacing, cw) in;

vec4 interpolate(vec4 v0, vec4 v1, vec4 v2, vec4 v3){
vec4 a = mix(v0, v1, gl_TessCoord.x);
vec4 b = mix(v2, v3, gl_TessCoord.x);
return mix(a,b,gl_TessCoord.y);
}

void main()
{
    gl_Position = interpolate(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position, gl_in[3].gl_Position);
}
