#include "Game.hpp"

Game::Game(unsigned short width, unsigned short height) {
    float pixelScale = 1.0;

    this->width  = width * pixelScale;
    this->height = height * pixelScale;
}

Game::~Game() {
    std::cout << __FUNCTION__ << std::endl;
    this->gc();
}

void Game::start() {
    this->shader->use();
    this->shader->setMat4("projection", worldMatrix);

    while (!glfwWindowShouldClose(this->window))
    {
        // input
        // -----
        processInput(window);
        // glfwSetKeyCallback(window, key_callback);

        // render
        // ------
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        float currentTime = (float)glfwGetTime();

        this->renderSprites(this->sprites, currentTime);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        GLenum error = glGetError();
        if (error != 0)
        {
            std::cout << "erro no opengl: " << error << std::endl;
        }
    }
}

void Game::setup() {
    this->setupGlfw();
    this->setupGlad();

    this->init();
}

void Game::init() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
        const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
        GLFWwindow *window = glfwCreateWindow(mode->width, mode->height, "My Title", glfwGetPrimaryMonitor(), NULL);

        // GLFWwindow *window = glfwCreateWindow(this->width, this->height, "O Jogo", NULL, NULL);
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

    this->window = window;
}

void Game::setupGlad() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        // return false;
        // TODO handle error
    }
}

void Game::setShader(Shader* shader) {
    this->shader = shader;
}

void Game::processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        this->quit();
    }
}

void Game::quit() {
    glfwSetWindowShouldClose(window, true);
}

void Game::setWorldMatrix(glm::mat4 worldMatrix) {
    this->worldMatrix = worldMatrix;
}

void Game::add(std::shared_ptr<Sprite> sprite)
{
    this->sprites.push_back(sprite);
}

void Game::remove(std::shared_ptr<Sprite> sprite)
{
    // this->sprites.remove();
}

void Game::renderSprites(std::vector<std::shared_ptr<Sprite>> sprites, float currentTime)
{
    for(auto it = sprites.cbegin(); it < sprites.cend(); it++) {
        (*it)->update(currentTime);
        (*it)->draw(this->shader);
    }
}

void Game::gc() {
    this->sprites.clear();
    // glfwTerminate();
}