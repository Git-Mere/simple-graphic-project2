#version 450 core
/**
 * \file fill_3d.vert
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */
precision highp float;

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjection;

out vec4 vColor;
out vec3 v_normal;

void main()
{
    v_normal  = aVertexNormal;
    vColor = vec4(abs(aVertexNormal), 1.0);
    gl_Position = uProjection * uViewMatrix * uModelMatrix * vec4(aVertexPosition, 1.0);
}
