#pragma once

#include "../app.hpp"
#include "Sprite.hpp"

class Sprite;

class Animation
{
public:
    Animation() {}
    virtual ~Animation() {}

    bool update(Sprite* sprite, float currentTime);
    virtual bool updateFrame(Sprite* sprite, float currentTime, float ellapsedTime, float ellapsedTimeTotal, glm::vec3 startLocation) = 0;

private:
    float startTime = -1.0f;
    float lastFrameTime = -1.0f;
    glm::vec3 startLocation;
};