#include "AnimationLancamentoObliquo.hpp"

AnimationLancamentoObliquo::AnimationLancamentoObliquo(float velocidadeInicial, float angulo ) {
    this->velocidadeInicial = velocidadeInicial;
    this->angulo = angulo;
    this->seno = sin (this->angulo * M_PI/180.0f);
    this->cosseno = cos (this->angulo * M_PI/180.0f);
    
}

AnimationLancamentoObliquo::~AnimationLancamentoObliquo() {
    std::cout << __FUNCTION__ << std::endl;
}

bool AnimationLancamentoObliquo::updateFrame(Sprite* sprite, float currentTime, float ellapsedTime, float ellapsedTimeTotal, glm::vec3 startLocation) {

    float speed = .01f;

    float posiX = this->velocidadeInicial * this->cosseno * ellapsedTimeTotal;
    // velocidade inicial em relação ao seno
    float velocidade = this->velocidadeInicial * this->seno;
    // velocidade atual 
    float velocidadeAtual = velocidade - (this->gravidade * ellapsedTimeTotal);
    //posição de Y
    float posiY = velocidadeAtual * ellapsedTimeTotal - (this->gravidade * (pow(ellapsedTimeTotal,2.0f)/2));
    
    sprite->translate(startLocation.x + posiX * speed, startLocation.y + posiY * speed);
    return (startLocation.y + posiY * speed) >= 0;
    // return sprite->getTranslateVec().y >= 0;
 
}