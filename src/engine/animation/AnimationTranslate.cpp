#include "AnimationTranslate.hpp"

AnimationTranslate::AnimationTranslate(float duration, float dxTotal, float dyTotal) {
    this->duration = duration;
    this->dxTotal = dxTotal;
    this->dyTotal = dyTotal;
}

AnimationTranslate::~AnimationTranslate() {
    std::cout << __FUNCTION__ << std::endl;
}

bool AnimationTranslate::updateFrame(Sprite* sprite, float currentTime, float ellapsedTime, float ellapsedTimeTotal, glm::vec3 startLocation) {
    float partial = 1.0f;
    if (ellapsedTimeTotal < this->duration) {
        partial = ellapsedTimeTotal / this->duration;
    }

    float dx = startLocation.x + this->dxTotal * partial;
    float dy = startLocation.y + this->dyTotal * partial;

    sprite->translate(dx, dy);

    return ellapsedTimeTotal < this->duration;
}