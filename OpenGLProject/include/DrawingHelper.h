#pragma once
#include <iostream>
#include "shaderClass.h"
#include "Scene.h"
#include "Camera.h"
#include "Model.h"

class DrawingHelper {
public:
    Shader setupShaderProgram(const std::string& shaderDir, const std::string& name) const;

    void drawScene(Shader& textureShader,
        Scene& scene,
        Camera& camera,
        Model* human);

    void floorTiles(Scene& scene);

    Model* humanModel();
};