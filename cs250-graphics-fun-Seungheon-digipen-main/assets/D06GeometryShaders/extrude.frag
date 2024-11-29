#version 450 core
/**
 * \file extrude.frag
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */

precision highp float;

layout(location=0)out vec4 fFragmentColor;

in vec2 gTextureCoordinates;
in vec3 gColor;

uniform sampler2D uTex2d;
uniform float uExtrudeFactor;

void main(){
    vec4 texture = texture(uTex2d,gTextureCoordinates);

    if(uExtrudeFactor > 0)
    {
         fFragmentColor = texture * vec4(gColor,1.0);
    }
    else{
        fFragmentColor = texture;
    }
}