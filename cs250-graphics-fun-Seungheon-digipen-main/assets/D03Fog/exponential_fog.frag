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
uniform float     uFogDensity;

void main()
{
    vec4 color = texture(uTex2d, vTextureCoordinates);

    #define LOG2 1.442695

    float fogDistance = length(v_position);
    float fogAmount = 1. - exp2(-uFogDensity * uFogDensity * fogDistance * fogDistance * LOG2);
    fogAmount = clamp(fogAmount, 0., 1.);

    fFragmentColor    = mix(color, vec4(uFogColor, 1.0), fogAmount);  
}