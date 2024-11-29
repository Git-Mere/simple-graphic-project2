#version 300 es
/**
 * \file
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */

layout(location = 0) in vec3 aVertexPosition;
layout(location = 2) in vec2 aVertexTextureCoordinates;

uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uProjection;

out vec2 vTextureCoordinates;
out vec3 v_position;

void main() {
  vec4 worldPosition = uModelMatrix * vec4(aVertexPosition, 1.0);
  vec4 toCam = uViewMatrix * worldPosition;
  gl_Position = uProjection * toCam;

  vTextureCoordinates = aVertexTextureCoordinates;

  v_position = (uViewMatrix * worldPosition).xyz;
}