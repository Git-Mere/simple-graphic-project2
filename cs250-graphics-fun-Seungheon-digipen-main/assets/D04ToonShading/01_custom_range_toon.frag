#version 300 es
precision highp float;
/**
 * \file
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */
layout(location=0)out vec4 fFragmentColor;

uniform vec3 uFogColor;
uniform float uFogNear;
uniform float uFogFar;
uniform vec3 uDiffuse;
uniform vec3 uAmbient;
uniform float uShininess;
uniform vec3 uSpecularColor;
uniform vec3 uLightDirection;

uniform float uLevel1;
uniform float uLevel2;
uniform float uLevel3;
uniform float uMax;
uniform bool uEnableAntiAliasing;

in vec3 NormalViewSpace;
in vec3 fog_position;

float stepmix(float edge0, float edge1, float E, float x)
{
    float T = clamp(0.5 * (x - edge0 + E) / E, 0.0, 1.0);
    return mix(edge0, edge1, T);
}


void main()
{
    vec3 N=normalize(NormalViewSpace);
    vec3 L=normalize(uLightDirection);
    vec3 E=vec3(0,0,1);
    vec3 H=normalize(L+E);
    
    float df=max(0.,dot(N,L));
    float sf=max(0.,dot(N,H));
    sf=pow(sf,uShininess);
    
    float A=uLevel1; //0.1
    float B=uLevel2; //0.3
    float C=uLevel3; //0.6
    float D=uMax;    //1.0
    
    if(uEnableAntiAliasing==true){
        float E = fwidth(df);
        
        if(df>A-E&&df<A+E) df = stepmix(A, B, E, df);
        else if(df>B-E&&df<B+E)df = stepmix(B, C, E, df);
        else if(df>C-E&&df<C+E)df = stepmix(C, D, E, df);
        else if(df<A)df=0.0;
        else if(df<B)df=B;
        else if(df<C)df=C;
        else df=D;
        E=fwidth(sf);
        if(sf>.5-E&&sf<.5+E)
        {
            sf=smoothstep(.5-E,.5+E,sf);
        }
        else
        {
            sf=step(.5,sf);
        }
        
    }
    else{
        if(df<A)df=0.;
        else if(df<B)df=B;
        else if(df<C)df=C;
        else df=D;
        sf=step(.5,sf);
    }

    
    vec3 color=uAmbient+df*uDiffuse+sf*uSpecularColor;
    float fogDistance = length(fog_position);
    float fogAmount=smoothstep(uFogNear,uFogFar,fogDistance);
    fFragmentColor=mix(vec4(color,1.f),vec4(uFogColor,1.),fogAmount);
    
}
