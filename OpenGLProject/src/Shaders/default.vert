#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Normals (not necessarily normalized)
layout (location = 1) in vec3 aNormal;
// Colors
layout (location = 2) in vec3 aColor;
// Texture Coordinates
layout (location = 3) in vec2 aTex;


// Outputs the current position for the Fragment Shader
out vec3 crntPos;
// Outputs the normal for the Fragment Shader
out vec3 Normal;
// Outputs the color for the Fragment Shader
out vec3 color;
// Outputs the texture coordinates to the Fragment Shader
out vec2 texCoord;



// Imports the camera matrix
uniform mat4 camMatrix;
// Imports the transformation matrices
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;
uniform float uScale;

void main()
{
    vec3 scaledPos = aPos * uScale;

    crntPos = vec3(model * translation * rotation * vec4(scaledPos, 1.0));

    Normal = mat3(transpose(inverse(model))) * aNormal;
    color = aColor;
    texCoord = vec2(aTex.x, 1.0 - aTex.y);

    gl_Position = camMatrix * vec4(crntPos, 1.0);
}