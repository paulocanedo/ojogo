#include "TransformAnimation.hpp"

void TransformAnimation::move(float startTime, float duration, float tx, float ty) {
    this->startTime = startTime;
    this->duration = duration;

    this->tx = tx;
    this->ty = ty;

    this->animating = true;
    this->transformation = TransformationType::translate;
}

void TransformAnimation::update(float currentTime) {
    if(!animating) return;
    
    float diff = currentTime - this->startTime;
    float percent = diff / this->duration;

    if(diff > this->duration) {
        this->finish();
    } else {
        switch(this->transformation) {
            case TransformationType::translate:
                float partialX = percent * this->tx;
                float partialY = percent * this->ty;
                this->sprite->translate(partialX, partialY);
                break;
        }
    }
}

void TransformAnimation::finish() {
    this->animating = false;
    this->sprite->translate(this->tx, this->ty);

    this->tx = 0.0f;
    this->ty = 0.0f;
}