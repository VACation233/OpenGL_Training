#version 330 core
struct Material
{
    sampler2D texture_diffuse1;
    //因为只有一个贴图，所以剩下的没用
    sampler2D texture_ambient1;
    sampler2D texture_specular1;
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

vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir)
{
    vec3 lightDir=normalize(-light.direction);
    //ambient
    vec3 ambient=light.ambient*vec3(texture(material.texture_diffuse1,TexCoords));
    //diffuse
    float diff=max(dot(normal,lightDir),0.0);
    vec3 diffuse=light.diffuse*diff*vec3(texture(material.texture_diffuse1,TexCoords));
    //specular
    vec3 reflectDir=reflect(-lightDir,normal);
    float spec=pow(max(dot(reflectDir,viewDir),0.0),material.shininess);
    vec3 specular=light.specular*spec*vec3(texture(material.texture_specular1,TexCoords));

    return ambient+diffuse+specular;
}

void main()
{    
    vec3 norm=normalize(Normal);
    vec3 viewDir=normalize(cameraPos-FragPos);
    vec3 lightResult=CalcDirLight(dirLight,norm,viewDir);

    vec3 result=lightResult;
    FragColor=mix(vec4(fogColor,1.0),vec4(result,1.0),visibility);
    //FragColor=vec4(result,1.0f);
    //FragColor = texture(material.texture_diffuse1, TexCoords);
    //FragColor=vec4(1.0,0,0,1);
}
