#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float timeGap;
uniform float timePeriod;


void main()
{
	gl_Position = projection * view * model * vec4(aPos.xyz, 1.0f);
	vec3 currentColor = vec3(aColor.xyz);
	float value;
	if(timeGap >= 0 && timeGap <= timePeriod / 3.0f)
	{
		value = (timeGap) / (timePeriod / 3.0f);
		if(aColor == vec3(1.0f, 0.0f, 0.0f))
		{
			currentColor = vec3(1-value, value, 0.0f);
		}
		else if(aColor == vec3(0.0f, 1.0f, 0.0f))
		{
			currentColor = vec3(0.0f, 1-value, value);
		}
		else if(aColor == vec3(0.0f, 0.0f, 1.0f))
		{
			currentColor = vec3(value, 0.0f, 1-value);
		}
	}
	else if(timeGap >= timePeriod / 3.0f && timeGap <= 2 * (timePeriod / 3.0f))
	{
		value = (timeGap-(timePeriod / 3.0f)) / (timePeriod / 3.0f);
		if(aColor == vec3(1.0f,0.0f,0.0f))
		{
			currentColor = vec3(0.0f, 1-value, value);
		}
		else if(aColor == vec3(0.0f, 1.0f, 0.0f))
		{
			currentColor = vec3(value, 0.0f, 1-value);
		}
		else if(aColor == vec3(0.0f, 0.0f, 1.0f))
		{
			currentColor = vec3(1-value, value, 0.0f);
		}
	}
	else if(timeGap >= 2 * (timePeriod / 3.0f) && timeGap <= timePeriod)
	{
		value = (timeGap-(2 * timePeriod / 3.0f)) / (timePeriod / 3.0f);
		if(aColor == vec3(1.0f, 0.0f, 0.0f))
		{
			currentColor = vec3(value, 0.0f, 1-value);
		}
		else if(aColor == vec3(0.0f, 1.0f, 0.0f))
		{
			currentColor = vec3(1-value, value, 0.0f);
		}
		else if(aColor == vec3(0.0f, 0.0f, 1.0f))
		{
			currentColor = vec3(0.0f, 1-value, value);
		}
	}
	ourColor = currentColor;
	TexCoord = aTexCoord;
}
