#include "Game.hpp"

Game::Game() {
    float pixelScale = 2.0;

    this->width  = 800 * pixelScale;
    this->height = 600 * pixelScale;
}

void Game::start() {
    this->setup();
}

void Game::setup() {
    this->setupGlfw();
    this->setupGlad();
}

void Game::setupGlfw() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
    #endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(this->width, this->height, "O Jogo", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        // return false;
        // TODO handle error
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow * window, int width, int height) {
        glViewport(0, 0, width, height);
    });
}

void Game::setupGlad() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        // return false;
        // TODO handle error
    }
}