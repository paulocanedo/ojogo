#include "AnimationTranslate.hpp"

AnimationTranslate::AnimationTranslate() {
}

bool AnimationTranslate::updateFrame(Sprite* sprite, float currentTime, float ellapsedTime, float ellapsedTimeTotal, glm::vec3 startLocation) {
    this->dx += ellapsedTime * 100.0f * (ellapsedTimeTotal > 3.0f ? -1.0f : 1.0f);

    float x = startLocation.x + this->dx;
    float y = startLocation.y;

    sprite->translate(x, y);

    return ellapsedTimeTotal < 10.0f;
}