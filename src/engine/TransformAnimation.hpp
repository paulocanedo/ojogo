#pragma once

#include "Sprite.hpp"

enum TransformationType {
    translate,
    scale,
    rotate
};


class TransformAnimation
{
public:
    TransformAnimation(Sprite *s) : sprite(s) { }

    void move(float startTime, float duration, float tx, float ty);
    void update(float currentTime);
    void finish();
private:
    Sprite *sprite;

    TransformationType transformation;

    bool animating = false;
    float tx;
    float ty;

    float startTime;
    float duration;
};