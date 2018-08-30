#include "Animation.hpp"

Animation::~Animation() {
}

bool Animation::update(Sprite* sprite, float currentTime)
{
    if (this->startTime == -1.0f)
    {
        this->startTime = currentTime;
        this->lastFrameTime = currentTime;
        this->startLocation = sprite->getTranslateVec();
    }
    float ellapsedTime = currentTime - lastFrameTime;
    float ellapsedTimeTotal = currentTime - this->startTime;

    bool result = this->updateFrame(sprite, currentTime, ellapsedTime, ellapsedTimeTotal, this->startLocation);

    this->lastFrameTime = currentTime;
    return result;
}