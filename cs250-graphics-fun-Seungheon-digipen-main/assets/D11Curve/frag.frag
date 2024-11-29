#version 450 core
/**
 * \file fill_3d.frag
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */

//rasterzation
//input = pixel's position(screen space)
//output = 그려지기로 결정된 pixel position 

//input = pixel position, interporatled attribute
//output = pixel color

precision highp float;

layout(location = 0) out vec4 fFragmentColor;

uniform vec3 uDiffuseMaterial;

in vec3 gColor;

void main()
{
    fFragmentColor = vec4(gColor, 1.0);
}
