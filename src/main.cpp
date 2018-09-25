#include "app.hpp"
#include "engine/Game.hpp"
#include "engine/Sprite.hpp"

#include "engine/animation/AnimationMultiImage.hpp"
#include "engine/animation/AnimationTranslate.hpp"
#include "engine/animation/AnimationQuedaLivre.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_HEIGHT = 1024;
const unsigned int SCR_WIDTH = SCR_HEIGHT * 16 / 9;

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

    std::shared_ptr<Sprite> goku = Sprite::fromTexture("./texturas/goku.png", 200.0f);
    goku->translate(500, 500);
    goku->rotate(3.14/4.0f);

    std::shared_ptr<Sprite> background = Sprite::fromTexture("./texturas/background.jpg", SCR_WIDTH);
    std::shared_ptr<Sprite> background2 = Sprite::fromTexture("./texturas/background_2.png", SCR_WIDTH);
    std::shared_ptr<Sprite> trees = Sprite::fromTexture("./texturas/trees.png", SCR_WIDTH);

    // std::shared_ptr<Sprite> bola = Sprite::fromTexture("./texturas/awesomeface.png", 32.0f);
    // bola->translate(700, SCR_HEIGHT - 30);

    // std::shared_ptr<Sprite> cat = Sprite::fromMultiImage("./texturas/sprite.png", 200.0f, 2, 4);
    // cat->translate(SCR_WIDTH - 300, 20);

    // std::unique_ptr<AnimationMultiImage> amt = std::make_unique<AnimationMultiImage>();
    // cat->animations.push_back(amt.get());

    glm::mat4 projection = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT, -1.0f, 1.0f);

    game->setShader(&shader);
    game->setWorldMatrix(projection);

    // game->add(background2);
    game->add(goku);
    // game->add(trees);
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
