#pragma once

#include "../Animation.hpp"
#include "../Sprite.hpp"

class AnimationTranslate : public Animation {

public:
    AnimationTranslate(Sprite* sprite);
    ~AnimationTranslate() {}

    bool update(float currentTime) override;

private:
    Sprite* sprite;
    float startTimeUpdate = -1.0f;
    glm::vec3 startLocation;
};