#version 330 core

out vec4 FragColor;

in vec3 crntPos;
in vec3 Normal;
in vec3 color;
in vec2 texCoord;

uniform vec4 lightColor;
uniform vec3 lightPos;

uniform sampler2D tex0;
uniform bool useTexture; // true if you want to sample the texture

void main()
{
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - crntPos); // vector from vertex to light
	
	float diffuse = max(dot(norm, lightDir), 0.0);
	float ambient = 0.3;
	
	vec3 lighting = (ambient + diffuse) * lightColor.rgb;
	
	vec4 baseColor = useTexture ? texture(tex0, texCoord) : vec4(color, 1.0);
	FragColor = vec4(baseColor.rgb * lighting, baseColor.a);
}
