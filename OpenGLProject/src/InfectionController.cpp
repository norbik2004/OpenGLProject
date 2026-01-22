#include "../include/InfectionController.h"
#include "../include/Mesh.h"

#include <algorithm>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

void InfectionController::init(Mesh& m)
{
    mesh = &m;

    glm::vec3 min(FLT_MAX);
    glm::vec3 max(-FLT_MAX);
    for (auto& v : m.vertices)
    {
        min = glm::min(min, v.position);
        max = glm::max(max, v.position);
    }

    startPoint = glm::vec3(
        (min.x + max.x) / 2.0f,
         min.y + (max.y - min.y) * 0.7f,
        (min.z + max.z) / 2.0f + 0.2f * (max.z - min.z)
    );


    size_t triCount = m.indices.size() / 3;
    infectionQueue.resize(triCount);
    for (size_t i = 0; i < triCount; ++i)
        infectionQueue[i] = i;

    std::sort(infectionQueue.begin(), infectionQueue.end(),
        [&m, this](size_t a, size_t b)
        {
            auto center = [&](size_t t)
                {
                    return (
                        m.vertices[m.indices[t * 3 + 0]].position +
                        m.vertices[m.indices[t * 3 + 1]].position +
                        m.vertices[m.indices[t * 3 + 2]].position
                        ) / 3.0f;
                };

            return glm::distance(center(a), startPoint)
                < glm::distance(center(b), startPoint);
        });

    currentIndex = 0;
    active = false;
    lastUpdate = std::chrono::steady_clock::now();
}

void InfectionController::start()
{
    if (active) return;
    active = true;
    lastUpdate = std::chrono::steady_clock::now();
}

void InfectionController::update()
{
    if (!active || !mesh || finished())
        return;

    auto now = std::chrono::steady_clock::now();
    float elapsed = std::chrono::duration<float>(now - lastUpdate).count();
    if (elapsed < updateInterval)
        return;

    size_t count = std::min(
        (size_t)trianglesPerUpdate,
        infectionQueue.size() - currentIndex
    );

    for (size_t i = 0; i < count; ++i)
    {
        size_t t = infectionQueue[currentIndex++];
        for (int j = 0; j < 3; ++j)
        {
            GLuint vIdx = mesh->indices[t * 3 + j];
            mesh->vertices[vIdx].color = glm::vec3(0.6f, 0.7f, 0.4f);
        }
    }

    mesh->UpdateVertices({});
    lastUpdate = now;
}

bool InfectionController::finished() const
{
    return currentIndex >= infectionQueue.size();
}
