#version 300 es
precision highp float;
/**
 * \file
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */
layout(location = 0) out vec4 fFragmentColor;

uniform vec3  uFogColor;
uniform float uFogNear;
uniform float uFogFar;

uniform vec3  uDiffuse;
uniform vec3  uAmbient;
uniform float uShininess;
uniform vec3  uSpecularColor;
uniform vec3  uLightDirection;
uniform int  uNumDiffuseChunks;

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
    vec3 N = normalize(NormalViewSpace);
    vec3 L = normalize(uLightDirection);
    vec3 E = vec3(0, 0, 1);
    vec3 H = normalize(L + E);
    
    float df = dot(N, L);
    float sf = max(0.0, dot(N, H));
    sf = pow(sf, uShininess);
    
    if (uEnableAntiAliasing) {
        float E = fwidth(df);
        bool only_one = false;
        float value = float(1) / float(uNumDiffuseChunks);
        for (int i = 0; i < uNumDiffuseChunks; i++) {
            float A = value * float(i);
            if(df > A - E && df < A + E){
                df = stepmix(A, A + value, E, df);
                only_one = true;
                break;
            }

        }
        if(only_one == false){
            df = ceil(df * float(uNumDiffuseChunks)) / float(uNumDiffuseChunks);         
        }

        E=fwidth(sf);
        if(sf>.5-E&&sf<.5+E)
        {
            sf=smoothstep(.5-E,.5+E,sf);
        }
        else
        {
            sf=step(.5,sf);
        }
    } else {      
        df = ceil(df * float(uNumDiffuseChunks)) / float(uNumDiffuseChunks);
        sf = step(0.5, sf);
    }
    
    vec3 color = uAmbient + df * uDiffuse + sf * uSpecularColor;
    float fogDistance = length(fog_position);
    float fogAmount=smoothstep(uFogNear,uFogFar,fogDistance);
    fFragmentColor = mix(vec4(color, 1.0), vec4(uFogColor, 1.0), fogAmount);
}