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

uniform vec3      uFogColor;
uniform float     uFogNear;
uniform float     uFogFar;

uniform vec3      uDiffuse;
uniform vec3      uAmbient;
uniform float     uShininess;
uniform vec3      uSpecularColor;
uniform vec3      uLightDirection;
uniform sampler2D uGradientTexture;
uniform bool      uEnableAntiAliasing;
uniform int       uImageWidth;

in vec3 NormalViewSpace;
in vec3 fog_position;

void main()
{
    vec3 N=normalize(NormalViewSpace);
    vec3 L=normalize(uLightDirection);
    vec3 E=vec3(0,0,1);
    vec3 H=normalize(L+E);
    
    float df=dot(N,L);
    float sf=max(0.,dot(N,H));
    sf=pow(sf,uShininess);
    
    vec4 tex = vec4(0);
    if(uEnableAntiAliasing==true){        
        float E = fwidth(df);

        bool only_one = false;
        float value = float(1) / float(uImageWidth); 

    if(uImageWidth % 2 == 0){      
        for (int i = 0; i < uImageWidth; i++) {
            float A = value * float(i);
            vec4 Atex = texture(uGradientTexture, vec2(A, 1));
            vec4 Atex_next = texture(uGradientTexture, vec2(A + value, 1));
            if(df > A + value - E && df < A + value + E){
                tex = mix(Atex, Atex_next, smoothstep(A + value - E, A + value + E, df));
                only_one = true;
                break;
            }
        }
        }
    else{
            for (int i = 0; i < uImageWidth; i++) {
            float A = value * float(i);
            vec4 Atex = texture(uGradientTexture, vec2(A, 1));
            vec4 Atex_next = texture(uGradientTexture, vec2(A + value, 1));
            if(df > A - E && df < A + E){
                tex = mix(Atex, Atex_next, smoothstep(A - E, A + E, df));
                only_one = true;
                break;
            }
        }
    }
        
        if(only_one == false){
            tex = texture(uGradientTexture, vec2(df, 1));
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
        
    }
    else{
        tex = texture(uGradientTexture, vec2(df, 1));
        sf=step(.5,sf);
    }
    vec3 color = uAmbient + tex.rgb * uDiffuse + sf * uSpecularColor;
    float fogDistance = length(fog_position);
    float fogAmount=smoothstep(uFogNear,uFogFar,fogDistance);
    fFragmentColor = mix(vec4(color, 1.0), vec4(uFogColor, 1.0), fogAmount);
}
