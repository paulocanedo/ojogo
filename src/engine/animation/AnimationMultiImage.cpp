#include "AnimationMultiImage.hpp"

const float AnimationMultiImage::DEFAULT_CHANGE_FRAME_TIME = 0.05f;

AnimationMultiImage::AnimationMultiImage(float changeFrameTime) {
    this->changeFrameTime = changeFrameTime;
}

AnimationMultiImage::~AnimationMultiImage() {
    std::cout << __FUNCTION__ << std::endl;
}

bool AnimationMultiImage::updateFrame(Sprite* sprite, float currentTime, float ellapsedTime, float ellapsedTimeTotal, glm::vec3 startLocation)
{
    if ((currentTime - this->lastFrameChange) > this->changeFrameTime) {
        sprite->nextFrame();
        this->lastFrameChange = currentTime;
    }

    return true;
}

void AnimationMultiImage::setChangeFrameTime(float time) {
    this->changeFrameTime = time;
}

float AnimationMultiImage::getChangeFrameTime() {
    return this->changeFrameTime;
}