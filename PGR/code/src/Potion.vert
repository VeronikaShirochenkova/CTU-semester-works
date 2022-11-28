#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;


smooth out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;
float decay = 0.05;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	float localTime = time * decay;

	vec2 offset = vec2((floor(localTime) - localTime) * 4 + 1.0, 0.0);

	TexCoord = aTexCoord + offset;
}
