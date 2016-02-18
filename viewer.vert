#version 410 core

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform vec3 light_position;

vec3 color;
layout(location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout(location = 2) in vec3 material_ambient;
layout (location = 3) in vec3 material_diffuse;

uniform bool condition;

out VS_OUT
{
     vec4 color0;
     vec4 color1;
} vs_out;

void main(void)
{
    vec4 world_pos = model_matrix * position;
    vec4 eye_pos = view_matrix * world_pos;
    vec4 clip_pos = projection_matrix * eye_pos;

    
    vec3 normal= mat3(view_matrix * model_matrix) * normal;
    vec3 N = normal;
    vec3 L = normalize(light_position);
    float LdotN = dot(N, L);
    float diffuse = max(LdotN, 0.0);
    
    color = vec3(material_ambient + LdotN * material_diffuse);
    
    if(condition==true){
    vs_out.color0=position * 2.0 +vec4(color,0.0);
    vs_out.color1=vec4(color,0.0)- position * 2.0;
    }
    
    else{
        vs_out.color0=  vec4(color,0.0);
        vs_out.color1 =vec4(color,0.0);
    }

    
    gl_Position = clip_pos;
}