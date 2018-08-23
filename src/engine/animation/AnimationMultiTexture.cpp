#include "AnimationMultiTexture.hpp"

const float AnimationMultiTexture::DEFAULT_CHANGE_FRAME_TIME = 0.05f;

AnimationMultiTexture::AnimationMultiTexture(float changeFrameTime)
{
    this->changeFrameTime = changeFrameTime;
}

bool AnimationMultiTexture::updateFrame(Sprite* sprite, float currentTime, float ellapsedTime, float ellapsedTimeTotal, glm::vec3 startLocation)
{
    if ((currentTime - this->lastFrameChange) > this->changeFrameTime) {
        sprite->nextFrame();
        this->lastFrameChange = currentTime;
    }

    return true;
}

void AnimationMultiTexture::setChangeFrameTime(float time) {
    this->changeFrameTime = time;
}

float AnimationMultiTexture::getChangeFrameTime() {
    return this->changeFrameTime;
}