#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location=2)in vec3 aNormal;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//ŒÌ–ß
out float visibility;
const float density=0.1;
const float gradient=1.5;


void main()
{
    Normal=mat3(transpose(inverse(model)))*aNormal;
    FragPos=vec3(model*vec4(aPos,1.0));

    TexCoords = aTexCoords*400;    
    //TexCoords=vec2(aTexCoords.x*40,(1-aTexCoords.y)*40);
    //TexCoords = aTexCoords;
    vec4 positionRelativeToCam=view*vec4(FragPos,1.0);
    gl_Position = projection * positionRelativeToCam;

    float dis=length(positionRelativeToCam.xyz);
    visibility=exp(-pow((dis*density),gradient));
    visibility=clamp(visibility,0.0,1.0);
}

