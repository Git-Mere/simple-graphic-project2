#version 450 core
/**
 * \file normals.geom
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */

precision highp float;

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in vec3 v_normal[];

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjection;


void main() {
    for (int i = 0; i < gl_in.length(); ++i) {
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
        
        mat3 normalMatrix = mat3(transpose(inverse(uViewMatrix * uModelMatrix)));
        vec3 normal = normalize(vec3(uProjection * vec4(normalMatrix * v_normal[i], 0.0)));
        gl_Position = gl_in[i].gl_Position + vec4(normal, 0.0) * 0.04;
        EmitVertex();

        EndPrimitive();
 
    }
}