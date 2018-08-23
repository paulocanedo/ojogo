#pragma once

#include "../Animation.hpp"

class AnimationMultiImage : public Animation {

public:
  AnimationMultiImage(float changeFrameTime = AnimationMultiImage::DEFAULT_CHANGE_FRAME_TIME);
  ~AnimationMultiImage();
  bool updateFrame(Sprite *sprite, float currentTime, float ellapsedTime, float ellapsedTimeTotal, glm::vec3 startLocation) override;

  void setChangeFrameTime(float time);
  float getChangeFrameTime();

private:
    static const float DEFAULT_CHANGE_FRAME_TIME;

    float lastFrameChange = -1.0f;
    float changeFrameTime = DEFAULT_CHANGE_FRAME_TIME;
};