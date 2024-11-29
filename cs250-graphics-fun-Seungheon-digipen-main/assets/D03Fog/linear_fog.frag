#version 300 es
precision highp float;
/**
 * \file
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */

in vec2 vTextureCoordinates;
in vec3 v_position;

layout(location = 0) out vec4 fFragmentColor;

uniform sampler2D uTex2d;
uniform vec3      uFogColor;
uniform float     uFogNear;
uniform float     uFogFar;

void main()
{
    vec4 color = texture(uTex2d, vTextureCoordinates);

      float fogDistance = length(v_position);
  float fogAmount = smoothstep(uFogNear, uFogFar, fogDistance);

    fFragmentColor    = mix(color, vec4(uFogColor, 1.0), fogAmount);  
}