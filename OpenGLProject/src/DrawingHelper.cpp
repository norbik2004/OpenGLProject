#include "../include/DrawingHelper.h"

Shader DrawingHelper::setupShaderProgram(const std::string& shaderDir, const std::string& name) const
{
    // Œcie¿ki do plików vertex + fragment
    std::string vertexPath = shaderDir + name + ".vert";
    std::string fragmentPath = shaderDir + name + ".frag";

    // Tworzymy shader i zwracamy
    Shader shaderProgram(vertexPath.c_str(), fragmentPath.c_str());
    return shaderProgram;
}