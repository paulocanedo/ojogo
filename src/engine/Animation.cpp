#include "Animation.hpp"

void Animation::setup(float startTime, float duration, std::function<void(glm::vec3, glm::vec3, float, float)> animationFunction)
{
    this->startTime = startTime;
    this->duration = duration;
    this->startLocation = sprite->getTranslateVec();
    this->startScale = sprite->getScaleVec();
    this->startAngle = sprite->getRotation();

    this->animationFunction = animationFunction;
    this->animating = true;
}

    void Animation::update(float currentTime)
{
    if(!animating) return;
    
    float diff = currentTime - this->startTime;
    float percent = diff / this->duration;

    if(diff > this->duration) {
        this->finish();
    } else {
        this->animationFunction(this->startLocation, this->startScale, this->startAngle, percent);
    }
}

void Animation::finish() {
    this->animationFunction(this->startLocation, this->startScale, this->startAngle, 1.0f);
    this->animating = false;
}