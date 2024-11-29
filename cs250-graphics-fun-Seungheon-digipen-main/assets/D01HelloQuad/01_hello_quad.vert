#version 300 es
precision highp float;

/**
 * \file
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexColor;
layout(location = 2) in vec2 aVertexTextureCoordinates;

out vec3 vColor;
out vec2 vTextureCoordinates;
uniform vec2 u_resolution;

uniform float u_time;

void main()
{
    float x = aVertexPosition.x;
    float y = aVertexPosition.y;
    float z = aVertexPosition.z;

    x = x * sin(u_time);

    gl_Position = vec4(x, y, z, 1.0);
    vColor = aVertexColor;
    vTextureCoordinates = aVertexTextureCoordinates;
}
