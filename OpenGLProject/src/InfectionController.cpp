#include "../include/InfectionController.h"
#include "../include/Mesh.h"

#include <algorithm>
#include <iostream>
#include <cfloat>

void InfectionController::init(Mesh& m)
{
    mesh = &m;

    glm::vec3 min(FLT_MAX);
    glm::vec3 max(-FLT_MAX);

    for (auto& v : m.getVertices())
    {
        min = glm::min(min, v.position);
        max = glm::max(max, v.position);
    }

    startPoint = glm::vec3(
        (min.x + max.x) * 0.5f,
        min.y + (max.y - min.y) * 0.7f,
        (min.z + max.z) * 0.5f + 0.2f * (max.z - min.z)
    );

    size_t triCount = m.getIndices().size() / 3;
    infectionQueue.resize(triCount);

    for (size_t i = 0; i < triCount; ++i)
        infectionQueue[i] = i;

    std::sort(infectionQueue.begin(), infectionQueue.end(),
        [this](size_t a, size_t b)
        {
            auto center = [&](size_t t)
                {
                    return (
                        mesh->getVertices()[mesh->getIndices()[t * 3 + 0]].position +
                        mesh->getVertices()[mesh->getIndices()[t * 3 + 1]].position +
                        mesh->getVertices()[mesh->getIndices()[t * 3 + 2]].position
                        ) / 3.0f;
                };

            return glm::distance(center(a), startPoint)
                < glm::distance(center(b), startPoint);
        });

    greenIndex = 0;
    blueIndex = 0;
    greenActive = false;
    blueActive = false;
    paused = false;

    lastUpdate = std::chrono::steady_clock::now();
}

void InfectionController::startGreen()
{
    greenIndex = 0;
    greenActive = true;
    paused = false;

    std::cout << "[Infection] Green started\n";
}

void InfectionController::startBlue()
{
    blueIndex = 0;
    if(greenActive)
        blueActive = true;
    paused = false;

    std::cout << "[Infection] Blue started\n";
}

void InfectionController::update(float deltaTime)
{
    if (!mesh || paused)
        return;

    bool dirty = false;

    if (greenActive)
        greenAccumulator += deltaTime;
    if (blueActive)
        blueAccumulator += deltaTime;

    if (greenActive && greenAccumulator >= updateInterval)
    {
        greenAccumulator = 0.0f;

        size_t count = std::min(
            (size_t)trianglesPerUpdate,
            infectionQueue.size() - greenIndex
        );

        for (size_t i = 0; i < count; ++i)
        {
            size_t t = infectionQueue[greenIndex++];
            for (int j = 0; j < 3; ++j)
            {
                GLuint vIdx = mesh->getIndices()[t * 3 + j];
                mesh->getVertices()[vIdx].color = greenColor;
            }
        }

        dirty = true;

        if (greenIndex >= infectionQueue.size())
            greenActive = false;
    }

    if (blueActive && blueAccumulator >= updateInterval)
    {
        blueAccumulator = 0.0f;

        size_t count = std::min(
            (size_t)trianglesPerUpdate,
            infectionQueue.size() - blueIndex
        );

        for (size_t i = 0; i < count; ++i)
        {
            size_t t = infectionQueue[blueIndex++];
            for (int j = 0; j < 3; ++j)
            {
                GLuint vIdx = mesh->getIndices()[t * 3 + j];
                mesh->getVertices()[vIdx].color = blueColor;
            }
        }

        dirty = true;

        if (blueIndex >= infectionQueue.size())
            blueActive = false;
    }

    if (dirty)
        mesh->UpdateVertices({});
}

void InfectionController::togglePause()
{
    paused = !paused;
    std::cout << "[Infection] "
        << (paused ? "Paused\n" : "Resumed\n");
}

void InfectionController::speedUp(float value)
{
    trianglesPerUpdate += value;
}

void InfectionController::speedDown(float value)
{
    trianglesPerUpdate -= value;
    if (trianglesPerUpdate < 10.0f)
        trianglesPerUpdate = 10.0f;
}

void InfectionController::fasterUpdate(float value)
{
    updateInterval -= value;
    if (updateInterval < 0.05f)
        updateInterval = 0.05f;
}

void InfectionController::slowerUpdate(float value)
{
    updateInterval += value;
}

void InfectionController::reset()
{
    if (!mesh)
        return;

    for (auto& v : mesh->getVertices())
        v.color = glm::vec3(1.0f);

    mesh->UpdateVertices({});

    greenIndex = 0;
    blueIndex = 0;
    greenActive = false;
    blueActive = false;
    paused = false;

    trianglesPerUpdate = 200.0f;
    updateInterval = 0.5f;

    std::cout << "[Infection] Reset complete\n";
}
