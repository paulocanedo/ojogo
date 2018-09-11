#include "../Animation.hpp"

class AnimationQuedaLivre : public Animation {

public:
    AnimationQuedaLivre(float yFinal = 0.0f);
    ~AnimationQuedaLivre();

    bool update(float currentTime);
    bool updateFrame(Sprite* sprite, float currentTime, float ellapsedTime, float ellapsedTimeTotal, glm::vec3 startLocation) override;

  private:
    float gravidade = 10.0f;

    float yFinal;
};