#pragma once
#include <vector>
#include <chrono>
#include <glm/glm.hpp>

class Mesh;

struct InfectionController
{
    Mesh* mesh = nullptr;
    std::vector<size_t> infectionQueue;
    size_t currentIndex = 0;

    float trianglesPerUpdate = 200.0f;
    float updateInterval = 0.5f;
    glm::vec3 startPoint;
    std::chrono::steady_clock::time_point lastUpdate;

    bool active = false;

    void init(Mesh& m);
    void start();
    void update();
    bool finished() const;
};