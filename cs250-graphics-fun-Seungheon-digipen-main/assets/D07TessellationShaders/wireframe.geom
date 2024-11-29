#version 450 core
/**
 * \file wireframe.geom
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */

precision highp float;

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;


void main() {
    for(int i = 0; i < gl_in.length(); i++){
        gl_Position = gl_in[i].gl_Position;
         EmitVertex();
    }
    gl_Position = gl_in[0].gl_Position;
         EmitVertex();


    EndPrimitive();
}