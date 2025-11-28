#pragma once

#include"../include/Scene.h"

void Scene::AddTextureMesh(Mesh* mesh)
{
	this->textureMeshes.push_back(mesh);
}

void Scene::AddColorMesh(Mesh* mesh)
{
	this->colorMeshes.push_back(mesh);
}

void Scene::AddLightMesh(Mesh* mesh)
{
	this->lightMeshes.push_back(mesh);
}

void Scene::RemoveTextureMesh(Mesh* mesh)
{
	this->textureMeshes.erase(std::remove(textureMeshes.begin(), textureMeshes.end(), mesh), textureMeshes.end());
}

void Scene::RemoveColorMesh(Mesh* mesh)
{
	this->colorMeshes.erase(std::remove(colorMeshes.begin(), colorMeshes.end(), mesh), colorMeshes.end());
}

void Scene::RemoveLightMesh(Mesh* mesh)
{
	this->lightMeshes.erase(std::remove(lightMeshes.begin(), lightMeshes.end(), mesh), lightMeshes.end());
}
