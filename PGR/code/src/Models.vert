#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;  

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec2 TexCoords;

out vec3 Tangent;
out vec3 Bitangent;
out vec3 Normal;

uniform bool scale;
uniform float time;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoords = aTexCoords; 
    Tangent = aTangent;
    Bitangent = aBitangent;
    Normal = aNormal;

    gl_Position = projection * view * model * vec4(aPos, 1.0);

    float tmp = time;
    if (scale) 
    {
        if (tmp < 3 && tmp > 0) {
            gl_Position = projection * view * model * vec4(aPos * 0.3 * tmp, 1.0);
        }
        else if (tmp >= 3 && tmp < 6) {
            gl_Position = projection * view * model * vec4(aPos * 0.3 * (6 - tmp), 1.0);
        }   
    }
}
