#version 450 core
/**
 * \file params_line.tesc
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */
//input = gl_in[] 사이즈의 크기는 patch_size에 따라 결정됨
//output = gl_out, outer/inner tesslleation levels

//excute단계
//input = outer/inner tesslleation levels
//output = gl_Tesscoord 이 값은 정규화됨.


layout(vertices = 4) out;
uniform vec2 uInsideTessellation;
uniform vec4 uOutsideTessellation;
 
in vec3 vColor[];
out vec3 tcColor[];

#define ID gl_InvocationID
 
void main()
{
    tcColor[gl_InvocationID] = vColor[gl_InvocationID];
    if (ID == 0) {
        gl_TessLevelOuter[0] = uOutsideTessellation[0].x;
        gl_TessLevelOuter[1] = uOutsideTessellation[1].x;
    }
    gl_out[ID].gl_Position = gl_in[ID].gl_Position;
}
