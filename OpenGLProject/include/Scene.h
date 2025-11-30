#pragma once

#include <vector>
#include "Mesh.h"

class Scene
{
public:
    // Wskaüniki na wszystkie mesh'e w scenie
    std::vector<Mesh*> textureMeshes;

    Scene() = default;

    // Dodaje mesh do sceny
    void AddTextureMesh(Mesh* mesh);
    // Usuwa mesh ze sceny
    void RemoveTextureMesh(Mesh* mesh);

};