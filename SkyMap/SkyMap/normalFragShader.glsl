#version 330 core

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_ambient1;
    float shininess;
};
struct DirLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec2 TexCoords;
//添加反射效果
in vec3 FragPos;
in vec3 Normal;
//添加雾效
uniform vec3 fogColor;
in float visibility;

uniform Material material;
uniform DirLight dirLight;

//添加反射效果
uniform samplerCube skybox;
uniform vec3 cameraPos;

vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse 
    float diff = max(dot(normal, lightDir), 0.0);
    // specular 
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine 
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
    return (ambient + diffuse + specular);
    
}
void main()
{    
    vec3 norm=normalize(Normal);
    vec3 viewDir=normalize(cameraPos-FragPos);

    vec3 lightResult=CalcDirLight(dirLight,norm,viewDir);
    vec3 I =normalize(FragPos-cameraPos);
    vec3 R=reflect(I,norm);
    vec3 reflectMap=vec3(texture(material.texture_ambient1,TexCoords));
    vec3 reflection=vec3(texture(skybox,R).rgb)*reflectMap;

    vec3 norResult=texture(material.texture_diffuse1, TexCoords).rgb;
    //vec3 result=reflection+norResult;
    vec3 result=reflection+lightResult;
    //FragColor=vec4(result,1.0);
    FragColor=mix(vec4(fogColor,1.0),vec4(result,1.0),visibility);
    //FragColor = texture(texture_diffuse1, TexCoords);
}






//#version 330 core
//out vec4 FragColor;
//
//in vec3 Normal;
//in vec3 Position;
//
//uniform vec3 cameraPos;
//uniform samplerCube skybox;
//
//void main()
//{    
//    vec3 I = normalize(Position - cameraPos);
//    vec3 R = reflect(I, normalize(Normal));
//    FragColor = vec4(texture(skybox, R).rgb, 1.0);
//}
//
//




