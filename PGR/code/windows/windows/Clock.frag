#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform int frame;


void main()
{

	vec4 texColor = texture(texture1, TexCoord);
    if(texColor.a < 0.2f)
        discard;
	
    FragColor = texColor;
}

