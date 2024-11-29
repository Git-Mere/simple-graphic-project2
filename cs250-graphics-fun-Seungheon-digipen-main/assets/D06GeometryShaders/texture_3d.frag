#version 450 core
/**
 * \file texture_3d.frag
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */

precision highp float;

layout(location = 0) out vec4 fFragmentColor;
in vec2 gTextureCoordinates;

uniform sampler2D uTex2d;

void main()
{
    fFragmentColor = texture(uTex2d, gTextureCoordinates);
}
