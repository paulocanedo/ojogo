#pragma once

#include <functional>
#include "Sprite.hpp"

enum TransformationType {
    translate,
    scale,
    rotate
};


class Animation
{
public:
    Animation(Sprite *s) : sprite(s) { }

    void setup(float startTime, float duration, std::function<void(glm::vec3, glm::vec3, float, float)>);
    void update(float currentTime);

private:
    void finish();

    Sprite *sprite;

    bool animating = false;
    std::function<void(glm::vec3, glm::vec3, float, float)>animationFunction;

    float startTime;
    float duration;
    glm::vec3 startLocation;
    glm::vec3 startScale;
    float startAngle;
};