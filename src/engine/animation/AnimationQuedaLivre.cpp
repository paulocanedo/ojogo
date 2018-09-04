#include "AnimationQuedaLivre.hpp"

AnimationQuedaLivre::AnimationQuedaLivre(float yFinal) {
    this->yFinal = yFinal;
}

AnimationQuedaLivre::~AnimationQuedaLivre() {
    std::cout << __FUNCTION__ << std::endl;
}

bool AnimationQuedaLivre::updateFrame(Sprite* sprite, float currentTime, float ellapsedTime, float ellapsedTimeTotal, glm::vec3 startLocation) {

    // float partial = ellapsedTimeTotal / this->duration;
    // float dx = this->dxTotal * partial;
    std::cout <<"tempo gasto last frame: " <<ellapsedTime << std::endl;
    std::cout <<"tempo da aplicação: " <<currentTime << std::endl;
    std::cout <<"tempo gasto total animação: " <<ellapsedTimeTotal << std::endl;
    float dy = this->g * (ellapsedTimeTotal*ellapsedTimeTotal);
    dy = dy/2.0f;
    std::cout <<"posição: " <<dy << std::endl;
    sprite->translate(startLocation.x, startLocation.y-dy);
    //return true;
    return sprite->getTranslateVec().y >this->yFinal;
}