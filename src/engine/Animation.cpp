#include "Animation.hpp"

bool Animation::update(Sprite* sprite, float currentTime)
{
    float startTime = this->startTime;

    if (startTime == -1.0f)
    {
        this->startTime = currentTime;
        this->lastFrameTime = currentTime;
        this->startLocation = sprite->getTranslateVec();
    }
    float ellapsedTime = currentTime - lastFrameTime;
    float ellapsedTimeTotal = currentTime - startTime;

    bool result = this->updateFrame(sprite, currentTime, ellapsedTime, ellapsedTimeTotal, this->startLocation);

    this->lastFrameTime = currentTime;
    return result;
}