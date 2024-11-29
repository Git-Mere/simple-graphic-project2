#version 450 core
/**
 * \file params_line.tesc
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */

layout(vertices = 4) out;
uniform vec2 uInsideTessellation;
uniform vec4 uOutsideTessellation;
 
#define ID gl_InvocationID
 
void main()
{
    if (ID == 0) {
        gl_TessLevelOuter[0] = uOutsideTessellation[0].x;
        gl_TessLevelOuter[1] = uOutsideTessellation[1].x;
    }
    gl_out[ID].gl_Position = gl_in[ID].gl_Position;
}
