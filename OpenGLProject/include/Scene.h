#pragma once

#include <vector>
#include "Mesh.h"

class Scene
{
public:
    // WskaŸniki na wszystkie mesh'e w scenie
    std::vector<Mesh*> meshes;

    Scene() = default;

    // Dodaje mesh do sceny
    void AddMesh(Mesh* mesh)
    {
        meshes.push_back(mesh);
    }

    // Usuwa mesh ze sceny
    void RemoveMesh(Mesh* mesh)
    {
        meshes.erase(std::remove(meshes.begin(), meshes.end(), mesh), meshes.end());
    }

    // Zwraca liczbê meshów
    size_t GetMeshCount() const
    {
        return meshes.size();
    }

    // Iteracja po wszystkich meshach
    std::vector<Mesh*>::iterator begin() { return meshes.begin(); }
    std::vector<Mesh*>::iterator end() { return meshes.end(); }

    std::vector<Mesh*>::const_iterator begin() const { return meshes.begin(); }
    std::vector<Mesh*>::const_iterator end() const { return meshes.end(); }
};