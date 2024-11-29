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
layout(location = 1) in vec3 aVertexNormal;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjection;
uniform mat3 uNormalMatrix;

out vec3 NormalViewSpace;
out vec3 fog_position;

void main()
{
    vec4 worldPosition = uModelMatrix * vec4(aVertexPosition, 1.0);
    vec4 clipPosition = uProjection * (uViewMatrix * worldPosition);
    gl_Position = clipPosition;

    NormalViewSpace = uNormalMatrix * aVertexNormal;

    fog_position = -(uViewMatrix * worldPosition).xyz;
}