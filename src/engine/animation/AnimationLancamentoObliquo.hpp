#include "../Animation.hpp"
#include <cmath>

class AnimationLancamentoObliquo : public Animation {

public:
    AnimationLancamentoObliquo(float velocidadeInicial = 0.0f, float angulo = 0.0f) ;
    ~AnimationLancamentoObliquo();

    bool update(float currentTime);
    bool updateFrame(Sprite* sprite, float currentTime, float ellapsedTime, float ellapsedTimeTotal, glm::vec3 startLocation) override;

  private:
    float gravidade = 10.0f;
    ///<summary>
    /// velocidade em metros por segundo
    ///<summary>
    float velocidadeInicial = 0.0f;
    float angulo = 0.0f;
    float seno = 0.0f;
    float cosseno = 0.0f;
    
};