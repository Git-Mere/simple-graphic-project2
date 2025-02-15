#version 300 es
/**
 * \file shadows.vert
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */

precision highp float;

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;

out vec3 vNormalInViewSpace;
out vec3 vPositionInViewSpace;
out vec4 vPositionInShadowSpace;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjection;
uniform mat3 uNormalMatrix;
uniform mat4 uShadowMatrix;

void main()
{
    gl_Position = uProjection * uViewMatrix * uModelMatrix * vec4(aVertexPosition, 1.0);
    vNormalInViewSpace = uNormalMatrix * aVertexNormal;
    vPositionInViewSpace = (uViewMatrix * uModelMatrix * vec4(aVertexPosition, 1.0)).xyz;
    vPositionInShadowSpace = uShadowMatrix * uModelMatrix * vec4(aVertexPosition, 1.0);
}
