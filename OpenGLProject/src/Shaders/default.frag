#version 330 core

out vec4 FragColor;

in vec3 crntPos;
in vec3 Normal;
in vec3 color;
in vec2 texCoord;

// U¿ywasz tylko diffuse
uniform sampler2D tex0;

// Œwiat³o
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

vec4 pointLight()
{	
	vec3 lightVec = lightPos - crntPos;

	// attenuation
	float dist = length(lightVec);
	float a = 0.05;
	float b = 0.01;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	// ambient
	float ambient = 0.20f;

	// diffuse
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	return texture(tex0, texCoord) * (diffuse * inten + ambient) * lightColor;
}

vec4 direcLight()
{
	float ambient = 0.10f;

	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	return texture(tex0, texCoord) * (diffuse + ambient) * lightColor;
}

vec4 spotLight()
{
	float outerCone = 0.90f;
	float innerCone = 0.95f;

	float ambient = 0.20f;

	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	return texture(tex0, texCoord) * (diffuse * inten + ambient) * lightColor;
}

void main()
{
	FragColor = direcLight();
}
