#version 330 core

out vec4 FragColor;

in vec3 crntPos;
in vec3 Normal;
in vec3 color;
in vec2 texCoord;

// Œwiat³o
uniform vec4 lightColor;
uniform vec3 lightPos;

// Tekstura (opcjonalna)
uniform sampler2D tex0;
uniform bool useTexture; // ustaw true jeœli chcesz teksturê

void main()
{
    // Normalne
    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(lightPos - crntPos);

    // Diffuse + ambient
    float diffuse = max(dot(norm, lightDir), 0.0);
    float ambient = 0.2;

    vec4 baseColor;
    if (useTexture) {
        baseColor = texture(tex0, texCoord);
        if(baseColor.a < 0.1) // jeœli brak tekstury, u¿yj kremowego
            baseColor = vec4(1.0, 0.9, 0.7, 1.0);
    } else {
        baseColor = vec4(1.0, 0.9, 0.7, 1.0); // kremowy
    }

    // Finalne œwiat³o
    FragColor = baseColor * (ambient + diffuse) * lightColor;
}
