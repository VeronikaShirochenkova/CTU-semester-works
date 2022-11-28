#version 330 core
out vec4 FragColor;

smooth in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec3 viewPos;
uniform vec4 potion_color;
uniform bool fog;
uniform float global_time;

void main()
{
    FragColor = mix(potion_color, texture(texture1, TexCoord), 0.5f);
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

        FragColor = mix(fog_colour, FragColor, fog_factor);
    }  

}
