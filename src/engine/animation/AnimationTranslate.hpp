#pragma once

#include "../Animation.hpp"

class AnimationTranslate : public Animation {

public:
    AnimationTranslate();
    ~AnimationTranslate() {}

    bool update(float currentTime);
    bool updateFrame(Sprite* sprite, float currentTime, float ellapsedTime, float ellapsedTimeTotal, glm::vec3 startLocation) override;

  private:
    float dx = 0.0f;
};