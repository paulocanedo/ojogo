#include "AnimationMultiTexture.hpp"

AnimationMultiTexture::AnimationMultiTexture(Sprite *sprite, float changeFrameTime)
{
    this->sprite = sprite;
    this->changeFrameTime = changeFrameTime;
}

bool AnimationMultiTexture::update(float currentTime) {
    if (startTimeUpdate == -1.0f) {
        startTimeUpdate = currentTime;
    }
    float ellapsedTime = currentTime - startTimeUpdate;

    if (ellapsedTime > this->changeFrameTime) {
        startTimeUpdate = currentTime; //reset time
        sprite->nextFrame();
    }

    return true;
}

void AnimationMultiTexture::setChangeFrameTime(float time) {
    this->changeFrameTime = time;
}

float AnimationMultiTexture::getChangeFrameTime() {
    return this->changeFrameTime;
}