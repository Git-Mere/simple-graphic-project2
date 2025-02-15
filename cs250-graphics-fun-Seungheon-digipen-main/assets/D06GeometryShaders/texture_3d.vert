#version 450 core
/**
 * \file texture_3d.frag
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */

precision highp float;

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;
layout(location = 2) in vec2 aVertexTextureCoordinates;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjection;

out vec3 for_calculate;
out vec3 just_normal;

out vec2 vTextureCoordinates;

void main()
{
    for_calculate = aVertexPosition;
    just_normal =  aVertexNormal;
    gl_Position         = uProjection * uViewMatrix * uModelMatrix * vec4(aVertexPosition, 1.0);
    vTextureCoordinates = aVertexTextureCoordinates;
}
