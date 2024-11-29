#version 450 core
/**
 * \file fill_3d.frag
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */

precision highp float;

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform float uShrinkFactor;

in vec4 vColor[];
out vec4 gColor;

vec4 Verts[3];
vec4 Center;

void EmitShrunkVertex(int i, float shrink){
	vec4 point = mix(Center, Verts[i], shrink);
	gl_Position = point;
	gColor = vColor[i];
	EmitVertex();
}

void main()
{
	Verts[0] = gl_in[0].gl_Position;
	Verts[1] = gl_in[1].gl_Position;
	Verts[2] = gl_in[2].gl_Position;
	
	Center = (Verts[0]+Verts[1]+Verts[2]) / 3;
	
	EmitShrunkVertex(0, uShrinkFactor);
	EmitShrunkVertex(1, uShrinkFactor);
	EmitShrunkVertex(2, uShrinkFactor);
	EndPrimitive();
}

