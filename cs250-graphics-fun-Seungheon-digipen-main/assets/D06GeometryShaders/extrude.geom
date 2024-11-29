#version 450 core
/**
 * \file extrude.geom
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */
precision highp float;

layout(triangles) in;
layout(triangle_strip, max_vertices = 14) out;

in vec2 vTextureCoordinates[];
in vec3 for_calculate[];
in vec3 just_normal[];

out vec2 gTextureCoordinates;
out vec3 gColor;

uniform bool uFlat;
uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjection;
uniform float uExtrudeFactor;

void common_emit(int i){
    gl_Position = gl_in[i].gl_Position;
    gTextureCoordinates = vTextureCoordinates[i];
    gColor = vec3(0.0);
    EmitVertex();
}


void normal_emit(int i, bool check){
    vec3 normal;
    if(check == true){
        normal = normalize(cross(for_calculate[1] - for_calculate[0], for_calculate[2] - for_calculate[0]));
    }
    else{
        normal = normalize(just_normal[i]);
    }
    vec3 pos = for_calculate[i] + normalize(normal) * uExtrudeFactor;
    gl_Position = uProjection * uViewMatrix * uModelMatrix * vec4(pos, 1.0);
    gTextureCoordinates = vTextureCoordinates[i];
    gColor = vec3(1.0);
    EmitVertex();
}


void main() {
    //base 3point
    for (int i = 0; i < 3; ++i) {
        common_emit(i);
    }

    //column 8point
    for (int i = 0; i < 3; ++i) {
        common_emit(i);
        normal_emit(i, uFlat);
    }
    common_emit(0);
    normal_emit(0, uFlat);

    //top 3point
    for (int i = 0; i < 3; ++i) {     
        normal_emit(i, uFlat);
    }

    EndPrimitive();
}




