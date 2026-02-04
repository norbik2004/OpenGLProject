#pragma once
#include <vector>
#include <chrono>
#include <glm/glm.hpp>

class Mesh;

struct InfectionController
{
    Mesh* mesh = nullptr;
    std::vector<size_t> infectionQueue;
    size_t greenIndex = 0;
    size_t blueIndex = 0;

    float trianglesPerUpdate = 200.0f;
    float updateInterval = 0.5f;
    glm::vec3 startPoint;
    std::chrono::steady_clock::time_point lastUpdate;

    glm::vec3 greenColor = glm::vec3(0.6f, 0.7f, 0.4f);
    glm::vec3 blueColor = glm::vec3(0.2f, 0.4f, 1.0f);

    float greenAccumulator = 0.0f;
    float blueAccumulator = 0.0f;

    bool greenActive = false;
    bool blueActive = false;
    bool paused = false;

    void init(Mesh& m);
    void reset();

    void startGreen();
    void startBlue();

    void togglePause();
    void speedUp(float value);
    void speedDown(float value);
    void fasterUpdate(float value);
    void slowerUpdate(float value);

    void update(float deltaTime);
};