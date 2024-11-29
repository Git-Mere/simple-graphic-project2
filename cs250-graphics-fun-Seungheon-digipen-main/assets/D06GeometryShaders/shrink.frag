#version 450 core
/**
 * \file shrink.frag
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */
precision highp float;

in vec4 gColor;

layout(location = 0) out vec4 fFragmentColor;

void main()
{
	fFragmentColor = gColor;
}
