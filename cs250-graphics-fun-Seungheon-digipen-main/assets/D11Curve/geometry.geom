#version 450 core
/**
 * \file points.geom
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */

//input = a Primitive type, gl_in[]
//output = multiple Pirmitives

precision highp float;

layout (lines, invocations = 10) in;
layout (points, max_vertices = 128) out;

in vec3 teColor[];
out vec3 gColor;

uniform vec4 uPosition, uPosition2, uPosition3, uPosition4, uPosition5, uPosition6, uPosition7, uPosition8, uPosition9, uPosition10, uPosition11;
uniform vec4 uDepart, uDepart2, uDepart3, uDepart4, uDepart5, uDepart6, uDepart7, uDepart8, uDepart9, uDepart10;
uniform vec4 uArrive, uArrive2, uArrive3, uArrive4, uArrive5, uArrive6, uArrive7, uArrive8, uArrive9, uArrive10;

uniform vec4 uOutsideTessellation;
uniform bool uCatmull;



void main() {
    #define SIZE 10
    int ID = gl_InvocationID;
    int total_ID = int(ceil(uOutsideTessellation[1].x));

    vec4 Departs[SIZE] = vec4[SIZE](uDepart,uDepart2,uDepart3,uDepart4,uDepart5,uDepart6,uDepart7,uDepart8,uDepart9,uDepart10);
    vec4 Arrives[SIZE] = vec4[SIZE](uArrive,uArrive2,uArrive3,uArrive4,uArrive5,uArrive6,uArrive7,uArrive8,uArrive9,uArrive10);
    vec4 Positions[SIZE + 1] = vec4[SIZE + 1](uPosition,uPosition2,uPosition3,uPosition4,uPosition5,uPosition6,uPosition7,uPosition8,uPosition9,uPosition10, uPosition11);
    vec3 Colors[SIZE + 1] = vec3[SIZE + 1](vec3(1.0, 0.0, 0.0), vec3(1.0, 0.647, 0.0), vec3(1.0, 1.0, 0.0), vec3(0.498, 1.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 1.0, 1.0), vec3(0.0, 0.0, 1.0), vec3(0.0, 0.0, 0.502), vec3(0.502, 0.0, 0.502), vec3(1.0, 0.753, 0.796), vec3(1.0, 0.0, 1.0) );

    vec4 first = vec4(0);
    vec4 last = vec4(0);  
    vec4 u = vec4(0);
    vec4 v = vec4(0);
    vec3 first_color = vec3(0);
    vec3 last_color = vec3(0);

    //hermit spline
    if(uCatmull == false){
        for(int i = 0; i < total_ID; i++){
            if(ID == i){
                first = Positions[i];
                first_color = Colors[i];
                last = Positions[i + 1];
                last_color = Colors[i + 1];
                u = Departs[i];
                v = Arrives[i];
                if(i + 1 == total_ID){
                    last = Positions[SIZE];
                    last_color = Colors[SIZE];
                    v = Arrives[SIZE - 1];
                }
            }
        }
    }
    //Catmull-Rom Splines
    else{
        for(int i = 0; i < total_ID; i++){
            if(ID == i){
                vec4 prev = Positions[i - 1];
                vec4 next = Positions[i + 1];
                vec4 curr = Positions[i];
                vec4 nextnext = Positions[i + 2];
                if(i + 2 == total_ID){
                    nextnext = Positions[SIZE];
                }
                else if(i + 1 == total_ID){
                    next = Positions[SIZE];
                }

                first = curr;
                first_color = Colors[i];
                last = next;
                last_color = Colors[i + 1];
                u = (prev + next) / 2;
                v = (curr + nextnext) / 2;
                if(i == 0){
                    u = Departs[0];
                }
                if(i + 1 == total_ID){
                    v = Arrives[SIZE - 1];
                    last_color = Colors[SIZE];
                }
            }
        }      

    }
    
///////////////////First/////////////////
//////////////////////////////////////////
    gl_PointSize = 6;
    gl_Position = first;
    gColor = first_color;
    EmitVertex();
//////////////////////////////////////////
    gl_PointSize = 2;
    float help = 1.f / 126.f;
    for(float i = 1; i <= 126; i++){
        float t = help * i;
        float s = 1 - t;
        vec4 h0 = s*s*(1 + 2*t) * first;
        vec4 h1 = t*t*(1 + 2*s) * last;
        vec4 h2 = s*s*t*u;
        vec4 h3 = -s*t*t*v;

        gl_Position = h0 + h1 + h2 + h3;
        gColor = mix(first_color, last_color, t);
        EmitVertex();
    }

//////////////////Last//////////////////////
////////////////////////////////////////////
    gl_PointSize = 6;
    gl_Position = last;
    gColor = last_color;
    EmitVertex();
////////////////////////////////////////////

    EndPrimitive();
}