#include "app.hpp"
#include "engine/Game.hpp"
#include "engine/Sprite.hpp"

#include "engine/animation/AnimationMultiImage.hpp"
#include "engine/animation/AnimationTranslate.hpp"
#include "engine/animation/AnimationQuedaLivre.hpp"
#include "engine/animation/AnimationBezier.hpp"
#include "engine/animation/AnimationLancamentoObliquo.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

// settings
const float SCR_HEIGHT = 6.0f;
const float SCR_WIDTH = SCR_HEIGHT * 16 / 9.0f;

int main()
{

 {   
    std::cout << "Window size: (" << SCR_WIDTH << "," << SCR_HEIGHT << ")" << std::endl;

    std::unique_ptr<Game> game = std::make_unique<Game>();
    game->setup();    
    // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // build and compile our shader program
    // ------------------------------------
    Shader shader("./shaders/simple.vert", "./shaders/simple.frag");

    std::shared_ptr<Sprite> chico = Sprite::fromTexture("./texturas/chico.png", 1.0f);
    std::shared_ptr<Sprite> indio = Sprite::fromTexture("./texturas/indio.png", 1.0f);
    std::shared_ptr<Sprite> flecha = Sprite::fromTexture("./texturas/flecha.png", .1f);
    std::shared_ptr<Sprite> aindio = Sprite::fromMultiImage("./texturas/indio_animacao_teste.png", 1.0f, 12, 6);
    chico->translate(1, 0.1);
    indio->translate(4, 0.1);
    aindio->translate(7, 0.1);
    // aindio->scale(960.0f / 240.0f,540.0 / 240.0f);
    flecha->translate(0, SCR_HEIGHT/2.0f);
    flecha->rotate(glm::radians(60.0f));

    std::shared_ptr<Sprite> background = Sprite::fromTexture("./texturas/background.jpg", SCR_WIDTH);
    std::shared_ptr<Sprite> background2 = Sprite::fromTexture("./texturas/background_2.png", SCR_WIDTH);
    std::shared_ptr<Sprite> trees = Sprite::fromTexture("./texturas/trees.png", SCR_WIDTH);

    // std::shared_ptr<Sprite> bola = Sprite::fromTexture("./texturas/awesomeface.png", 32.0f);
    // bola->translate(700, SCR_HEIGHT - 30);

    // std::shared_ptr<Sprite> cat = Sprite::fromMultiImage("./texturas/sprite.png", 200.0f, 2, 4);
    // cat->translate(SCR_WIDTH - 300, 20);

    // std::unique_ptr<AnimationMultiImage> amt = std::make_unique<AnimationMultiImage>();
    // cat->animations.push_back(amt.get());

    std::unique_ptr<AnimationMultiImage> amt = std::make_unique<AnimationMultiImage>(0.042f);
    aindio->animations.push_back(amt.get());

    //  std::unique_ptr<AnimationBezier> at = std::make_unique<AnimationBezier>(glm::vec2(0.0f, 0.0f),glm::vec2(1000.0f, 350.0f),glm::vec2(0.0f, 700.0f),glm::vec2(500.0f, 0.0f),10);
    //  goku->animations.push_back(at.get());

    std::unique_ptr<AnimationLancamentoObliquo> aflecha = std::make_unique<AnimationLancamentoObliquo>(150.0f, 30.0f);
    // std::unique_ptr<AnimationLancamentoObliquo> aflecha = std::make_unique<AnimationLancamentoObliquo>(150.0f, glm::radians(30.0f));
    flecha->animations.push_back(aflecha.get());

    glm::mat4 projection = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT, -1.0f, 1.0f);

    game->setShader(&shader);
    game->setWorldMatrix(projection);

    game->add(background2);
    game->add(trees);
    game->add(chico);
    game->add(indio);
    game->add(flecha);
    game->add(aindio);
    game->start();
}

    std::cout << "finish him!" << '\n';
    // glfwTerminate(); TODO descobrir onde colocar isso aqui
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
//     if (action != GLFW_PRESS) return;

//     float duration = 0.5f;
//     float distance = 300.0f;
//     bool right = true;
//     bool flag = false;

//     switch (key)
//     {
//     case GLFW_KEY_RIGHT:
//         right = true;
//         flag = true;
//         std::cout << "pressed key: right, " << action << std::endl;
//         break;
//     case GLFW_KEY_DOWN:
//         std::cout << "pressed key: down, " << action << std::endl;
//         break;
//     case GLFW_KEY_LEFT:
//         right = false;
//         flag = true;
//         std::cout << "pressed key: left, " << action << std::endl;
//         break;
//     case GLFW_KEY_UP:
//         std::cout << "pressed key: up, " << action << std::endl;
//         break;
//     default:
//         break;
//     }

//     if(flag) {
//         goku->animations.push_back(new AnimationTranslate(duration, distance * (right ? 1.0f : -1.0f), 0.0f));
//     }
// }
