#version 330 core
struct Material
{
    sampler2D texture_diffuse1;
    //树只有一个贴图，所以剩下的基本上不会被使用，但还是加上去了
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
in vec3 Normal;
in vec3 FragPos;

uniform Material material;
uniform DirLight dirLight;
uniform vec3 cameraPos;

//雾效
uniform vec3 fogColor;
in float visibility;


vec4 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir)
{
    vec4 textureColor=texture(material.texture_diffuse1,TexCoords);
    if(textureColor.a<0.1)
        discard;
    vec3 lightDir=normalize(-light.direction);
    //diffuse
    float diff=max(dot(lightDir,normal),0.0);

    //specular
    vec3 reflectDir=reflect(-lightDir,normal);
    float spec=pow(max(dot(viewDir,reflectDir),0.0),material.shininess);

//    vec3 ambient=light.ambient*vec3(texture(material.texture_diffuse1,TexCoords));
//    vec3 diffuse=light.diffuse*diff*vec3(texture(material.texture_diffuse1,TexCoords));
//    vec3 specular=light.specular*spec*vec3(texture(material.texture_specular1,TexCoords));
    vec4 ambient=vec4(light.ambient,1.0)*textureColor;
    vec4 diffuse=vec4(light.diffuse,1.0)*diff*textureColor;
    vec4 specular=vec4(light.specular,1.0)*spec*textureColor;

    return (ambient+diffuse+specular);

}

void main()
{    
    
    vec3 norm=normalize(Normal);
    vec3 viewDir=normalize(cameraPos-FragPos);
    
    vec4 lightResult=CalcDirLight(dirLight,norm,viewDir);

    
    vec4 result=lightResult;


    FragColor=mix(vec4(fogColor,1.0),result,visibility);
    //FragColor = texture(material.texture_diffuse1, TexCoords);
}




