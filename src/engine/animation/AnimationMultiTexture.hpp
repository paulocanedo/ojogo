#pragma once

#include "../Animation.hpp"
#include "../Sprite.hpp"

class AnimationMultiTexture : public Animation {

public:
    AnimationMultiTexture(Sprite* sprite, float changeFrameTime = 0.05f);
    ~AnimationMultiTexture() {}
    bool update(float currentTime) override;

    void setChangeFrameTime(float time);
    float getChangeFrameTime();

private:
    Sprite *sprite;
    float startTimeUpdate = -1.0f;

    float changeFrameTime = 0.05f;
};