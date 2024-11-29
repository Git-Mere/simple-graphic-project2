#version 450 core
/**
 * \file params_tri_spacing_odd.tese
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */

layout(triangles, fractional_odd_spacing, cw) in;

void main()
{
	vec4 p0 = gl_TessCoord.x * gl_in[0].gl_Position;
	vec4 p1 = gl_TessCoord.y * gl_in[1].gl_Position;
	vec4 p2 = gl_TessCoord.z * gl_in[2].gl_Position;

	vec4 tePosition = normalize(p0 + p1 + p2);

    gl_Position = tePosition;
}
