#version 450 core
/**
 * \file points.geom
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */

precision highp float;

layout (lines) in;
layout (points, max_vertices = 128) out;


void main() {
    gl_PointSize = 6;
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gl_PointSize = 1;
    float help = 1.f / 126.f;
    for(float i = 1; i <= 126; i++){
        gl_Position = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, help * i);
        EmitVertex();
    }

    gl_PointSize = 6;
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();


    EndPrimitive();
}