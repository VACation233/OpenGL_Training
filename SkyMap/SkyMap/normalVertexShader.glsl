//
//
//#version 330 core
//layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aNormal;
//
//out vec3 Normal;
//out vec3 Position;
//
//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;
//
//void main()
//{
//    Normal = mat3(transpose(inverse(model))) * aNormal;
//    Position = vec3(model * vec4(aPos, 1.0));
//    gl_Position = projection * view * model * vec4(aPos, 1.0);
//}
//



#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
//制作反射效果
out vec3 Normal;
out vec3 FragPos;
//雾效
out float visibility;
const float density=0.1;
const float gradient=1.5;



uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    
    Normal=mat3(transpose(inverse(model)))*aNormal;
    FragPos=vec3(model*vec4(aPos,1.0));
    //计算相对位置
    vec4 positionRelativeToCam=view*vec4(FragPos,1.0);
    TexCoords = vec2(aTexCoords.x,  aTexCoords.y);  
    gl_Position = projection * positionRelativeToCam;
    float dis=length(positionRelativeToCam.xyz);
    visibility=exp(-pow((dis*density),gradient));
    visibility=clamp(visibility,0.0,1.0);
}




