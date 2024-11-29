#version 300 es
precision highp float;

/**
 * \file
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */

in vec3 vColor;
in vec2 vTextureCoordinates;

uniform sampler2D uTex2d;

layout(location = 0) out vec4 fFragmentColor;

uniform float u_time;
uniform vec2 u_resolution;

void main()
{
vec2 coord = (gl_FragCoord.xy * 2.0 - u_resolution) / min(u_resolution.x, u_resolution.y);

float light = 0.0;
light = (0.3f / length(coord)) ;

    vec3  color       = mix(vColor, texture(uTex2d, vTextureCoordinates).rgb, sin(u_time));
    fFragmentColor    = vec4( color * vec3(light), 1.0);
}
