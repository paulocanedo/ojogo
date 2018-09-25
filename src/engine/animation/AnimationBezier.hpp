#pragma once
#include "../Animation.hpp"
#include <cmath>
#include "../../bezier.h"

class AnimationBezier : public Animation
{

private:
  float posiX;
  float posiY;
  glm::vec2 posi1;
  glm::vec2 posi2;
  glm::vec2 posi3;
  glm::vec2 posi4;
  
  float duracao;

  public:
  inline AnimationBezier(glm::vec2 posi1,glm::vec2 posi2,glm::vec2 posi3,glm::vec2 posi4,float duracao)
  {
    this->posi1 = posi1;
    this->posi2 = posi2;
    this->posi3 = posi3;
    this->posi4 = posi4;
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
    cubic_bezier_curve(this->posi1.x, this->posi1.y, this->posi2.x, this->posi2.y, this->posi3.x, this->posi3.y, this->posi4.x, this->posi4.y, partial, this->posiX, this->posiY);

    sprite->translate(startLocation.x + this->posiX, startLocation.y + this->posiY);
    return ellapsedTimeTotal < this->duracao;
  }
};