#pragma once
#include "../Animation.hpp"
#include <cmath>
#include "../../bezier.h"

class AnimationBezier : public Animation
{

private:
  float posiX;
  float posiY;
  float duracao;

  public:
  inline AnimationBezier(float duracao)
  {
    this->duracao = duracao;
  }
  inline ~AnimationBezier(){}

  bool update(float currentTime){return true;}

  bool updateFrame(Sprite *sprite, float currentTime, float ellapsedTime, float ellapsedTimeTotal, glm::vec3 startLocation)
  {

    float partial = 1.0f;
    if (ellapsedTimeTotal < this->duracao)
    {
      partial = ellapsedTimeTotal / this->duracao;
    }
    cubic_bezier_curve(0.0f, 0.0f, 1000.0f, 350.0f, 0.0f, 700.0f, 500.0f, 0.0f, partial, this->posiX, this->posiY);

    sprite->translate(startLocation.x + this->posiX, startLocation.y + this->posiY);
   
    return ellapsedTimeTotal < this->duracao;
    return true;
  }
};