#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform float currentTime;

out vec3 fragColor;

void main()
{
	float sin = -sin(5*currentTime);
	float cos = cos(5*currentTime);
	mat2 rot = mat2(cos, sin, -sin, cos);
	vec2 pos = rot * position.xy;
	gl_Position = vec4(pos, position.z, 1);
	fragColor = color;
}