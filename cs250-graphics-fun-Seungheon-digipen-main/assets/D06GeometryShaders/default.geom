#version 450 core
/**
 * \file default.geom
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */
precision highp float;

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec2 vTextureCoordinates[];

out vec2 gTextureCoordinates;

void main() {
    for (int i = 0; i < gl_in.length(); ++i) {
        gl_Position = gl_in[i].gl_Position;
        gTextureCoordinates = vTextureCoordinates[i];
        EmitVertex();
    }
    EndPrimitive();
}
