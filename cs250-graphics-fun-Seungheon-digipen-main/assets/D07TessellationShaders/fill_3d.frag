#version 450 core
/**
 * \file fill_3d.frag
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */

precision highp float;

layout(location = 0) out vec4 fFragmentColor;

uniform vec3 uDiffuseMaterial;

void main()
{
    fFragmentColor = vec4(uDiffuseMaterial, 1.0);
}
