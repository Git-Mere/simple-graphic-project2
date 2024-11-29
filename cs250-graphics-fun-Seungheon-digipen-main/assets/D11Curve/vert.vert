#version 450 core
/**
 * \file simple.vert
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */
//input = single vertex(model space)
//output = gl_Position(clip space)

precision highp float;

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexColor;
layout(location = 2) in vec2 aVertexTextureCoordinates;

uniform mat4 uProjection;

out vec3 vColor;

void main()
{
    gl_Position         = uProjection * vec4(aVertexPosition, 1.0);
    vColor = aVertexColor;
}
