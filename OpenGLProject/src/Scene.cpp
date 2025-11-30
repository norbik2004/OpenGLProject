#pragma once

#include"../include/Scene.h"

void Scene::AddTextureMesh(Mesh* mesh)
{
	this->textureMeshes.push_back(mesh);
}

void Scene::RemoveTextureMesh(Mesh* mesh)
{
	this->textureMeshes.erase(std::remove(textureMeshes.begin(), textureMeshes.end(), mesh), textureMeshes.end());
}
