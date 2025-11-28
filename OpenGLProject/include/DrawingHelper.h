#pragma once
#include <iostream>
#include "shaderClass.h"
#include "Scene.h"
#include "Camera.h"

class DrawingHelper {
public:
    Shader setupShaderProgram(const std::string& shaderDir, const std::string& name) const;

    void drawScene(Shader& textureShader,
        Shader& colorShader,
        Shader& lightShader,
        Scene& scene,
        Camera& camera);
};