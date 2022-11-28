#version 330 core
out vec4 FragColor;

#define NR_POINT_LIGHTS 7

struct Material {
    sampler2D texture_diffuse;
    sampler2D texture_specular;    
    sampler2D texture_normal;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

in vec3 FragPos;
in vec2 TexCoords;

in vec3 Tangent;
in vec3 Bitangent;
in vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPos;
uniform Material material;

uniform bool light_on;
uniform bool flashlight;
uniform bool fog;
uniform float global_time;


vec3 CalcDirLight(DirLight light, vec3 viewDir, mat3 TBN, vec3 TangentFragPos);
vec3 CalcPointLight(PointLight light, vec3 fragPos, vec3 viewDir, mat3 TBN, vec3 TangentFragPos);
vec3 CalcSpotLight(SpotLight light, vec3 fragPos, vec3 viewDir, mat3 TBN, vec3 TangentFragPos);


void main()
{    
    //====================TBN================================
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * Tangent);
    vec3 N = normalize(normalMatrix * Normal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    mat3 TBN = transpose(mat3(T, B, N));    

    vec3 TangentViewPos  = TBN * viewPos;
    vec3 TangentFragPos  = TBN * FragPos;
    //=======================================================

    vec3 norm = texture(material.texture_normal, TexCoords).rgb;
    norm = normalize(norm * 2.0 - 1.0);
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    
    // phase 1: directional lighting
    vec3 result = CalcDirLight(dirLight, viewDir, TBN, TangentFragPos);

    // phase 2: point lights
    if (light_on) {
        for(int i = 0; i < 7; i++)
            result += CalcPointLight(pointLights[i], FragPos, viewDir, TBN, TangentFragPos); 
    }
       
    // phase 3: spot light
    if (flashlight) {
        result += CalcSpotLight(spotLight, FragPos, viewDir, TBN, TangentFragPos);
    }
     
    FragColor = vec4(result, 1.0);


    if (fog) 
    {
        float fog_maxdist = 8.0;
        float fog_mindist = 2.0;
        vec4  fog_colour = vec4(0.4, 0.4, 0.4, 1.0);


        float dist = length(viewPos);
        float fog_factor = (fog_maxdist - dist) /
                          (fog_maxdist - fog_mindist);
  
        if (global_time < 12) {
            fog_factor = clamp(fog_factor + 0.04*global_time, 0.0, 1.0);
        }
        else {
            fog_factor = clamp(fog_factor + 0.04*(23-global_time), 0.0, 1.0);
        }
    

        FragColor = mix(fog_colour, vec4(result, 1.0), fog_factor);
    }
    
}


vec3 CalcDirLight(DirLight light, vec3 viewDir, mat3 TBN, vec3 TangentFragPos)
{
    
    vec3 TangentLightPos = TBN * light.direction;

    vec3 norm = texture(material.texture_normal, TexCoords).rgb;
    norm = normalize(norm * 2.0 - 1.0);

    // ambient
    vec3 ambient = light.ambient * texture(material.texture_diffuse, TexCoords).rgb;

    // diffuse shading
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse, TexCoords).rgb;

    // specular shading
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 halfwayDir = normalize(lightDir + viewDir); 
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.texture_specular, TexCoords).rgb;
    
    return (ambient + diffuse + specular);
}


vec3 CalcPointLight(PointLight light, vec3 fragPos, vec3 viewDir, mat3 TBN, vec3 TangentFragPos)
{
    vec3 TangentLightPos = TBN * light.position;

    vec3 norm = texture(material.texture_normal, TexCoords).rgb;
    norm = normalize(norm * 2.0 - 1.0);

    // ambient
    vec3 ambient = light.ambient * texture(material.texture_diffuse, TexCoords).rgb;

    // diffuse shading
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse, TexCoords).rgb;

    // specular shading
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 halfwayDir = normalize(lightDir + viewDir); 
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.texture_specular, TexCoords).rgb;

    // attenuation
    float distance = length(TangentLightPos - TangentFragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}


vec3 CalcSpotLight(SpotLight light, vec3 fragPos, vec3 viewDir, mat3 TBN, vec3 TangentFragPos)
{
    
    vec3 TangentLightPos = TBN * light.position;

    vec3 norm = texture(material.texture_normal, TexCoords).rgb;
    norm = normalize(norm * 2.0 - 1.0);

    // ambient
    vec3 ambient = light.ambient * texture(material.texture_diffuse, TexCoords).rgb;

    // diffuse shading
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse, TexCoords).rgb;

    // specular shading
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 halfwayDir = normalize(lightDir + viewDir); 
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.texture_specular, TexCoords).rgb;

    // attenuation
    float distance = length(TangentLightPos - TangentFragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-TBN*light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}
