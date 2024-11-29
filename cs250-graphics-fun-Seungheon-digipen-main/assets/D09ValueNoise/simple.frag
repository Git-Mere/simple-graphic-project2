#version 450 core
/**
 * \file simple.frag
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */

precision highp float;

layout(location = 0) out vec4 fFragmentColor;
in vec2 vTextureCoordinates;

uniform sampler2D uTex2d;

void main()
{
    vec4 color = texture(uTex2d, vTextureCoordinates);
    fFragmentColor = vec4(color.rgb, 1.f);
}
