#pragma once

#include <vector>
#include "Mesh.h"

class Scene
{
public:
    // Wskaüniki na wszystkie mesh'e w scenie
    std::vector<Mesh*> textureMeshes;
    std::vector<Mesh*> colorMeshes;
    std::vector<Mesh*> lightMeshes;

    Scene() = default;

    // Dodaje mesh do sceny
    void AddTextureMesh(Mesh* mesh);
    void AddColorMesh(Mesh* mesh);
    void AddLightMesh(Mesh* mesh);


    // Usuwa mesh ze sceny
    void RemoveTextureMesh(Mesh* mesh);
    void RemoveColorMesh(Mesh* mesh);
    void RemoveLightMesh(Mesh* mesh);

};