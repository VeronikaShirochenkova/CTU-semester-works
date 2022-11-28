#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform int frame;

vec2 CalcTexCoords(int, vec2);

void main()
{
	vec2 c = CalcTexCoords(frame, TexCoord);

	vec4 texColor = texture(texture1, c);
    if(texColor.a < 0.2f)
        discard;
	
    FragColor = texColor;
}

vec2 CalcTexCoords(int f, vec2 crds) 
{
	int cel = 5 - f / 6;
	int ost = f % 6;


	crds.x += ost * 1.0f/6;
	crds.y += cel * 1.0f/6;

	return crds;
}
