#pragma once
#include <iostream>
#include "shaderClass.h"

class DrawingHelper {
public:
    Shader setupShaderProgram(const std::string& shaderDir, const std::string& name) const;
};