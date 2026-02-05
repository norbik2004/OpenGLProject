#pragma once

#include <vector>
#include "Mesh.h"

class Scene
{
public:

    Scene() = default;

    // Dodaje mesh do sceny
    void AddTextureMesh(Mesh* mesh);
    // Usuwa mesh ze sceny
    void RemoveTextureMesh(Mesh* mesh);
    const std::vector<Mesh*>& getTextureMeshes() const { return textureMeshes; }

private:
    std::vector<Mesh*> textureMeshes;
};