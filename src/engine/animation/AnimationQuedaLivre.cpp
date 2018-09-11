#include "AnimationQuedaLivre.hpp"

AnimationQuedaLivre::AnimationQuedaLivre(float yFinal) {
    this->yFinal = yFinal;
}

AnimationQuedaLivre::~AnimationQuedaLivre() {
    std::cout << __FUNCTION__ << std::endl;
}

bool AnimationQuedaLivre::updateFrame(Sprite* sprite, float currentTime, float ellapsedTime, float ellapsedTimeTotal, glm::vec3 startLocation) {
    float aceleracao = this->gravidade * (ellapsedTimeTotal*ellapsedTimeTotal);
    float distancia = aceleracao/2.0f;
    sprite->translate(startLocation.x, startLocation.y-aceleracao);
    return sprite->getTranslateVec().y >this->yFinal;
}