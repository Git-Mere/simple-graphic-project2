#version 450 core
/**
 * \file fun.geom
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */

 //If I'm not in midterm period, I would have showed more fun thing

precision highp float;

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 vTextureCoordinates[];

out vec2 gTextureCoordinates;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjection;
uniform float uTime;

float random2d(vec2 coord){
    return abs(fract(cos(dot(coord.xy, vec2(12.9898, 78.233))) * -43758.5453));
}

void main() {
    for (int i = 0; i < gl_in.length(); ++i) {
        float random = fract(random2d(floor(gl_in[i].gl_Position.xy)) + uTime / 6.0);
        gl_Position = gl_in[i].gl_Position;
        gTextureCoordinates = vTextureCoordinates[i] / vec2(random) * sin(uTime);
        EmitVertex();
    }
    EndPrimitive();
}
