#include "AnimationTranslate.hpp"

AnimationTranslate::AnimationTranslate(Sprite* sprite) {
    this->sprite = sprite;
}

bool AnimationTranslate::update(float currentTime) {
    float startTime = this->startTimeUpdate;

    if (startTime == -1.0f)
    {
        this->startTimeUpdate = currentTime;
        this->startLocation = sprite->getTranslateVec();
    }
    float ellapsedTime = currentTime - startTime;

    float x = this->startLocation.x + (ellapsedTime * 200.0f);
    float y = this->startLocation.y;

    std::cout << "(" << x << "," << y << ")" << std::endl;

    sprite->translate(x, y);

    return ellapsedTime < 10.0f;
}