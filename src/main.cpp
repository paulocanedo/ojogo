#include "app.hpp"
#include "engine/Sprite.hpp"
#include "engine/Texture.hpp"
// #include "engine/Animation.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_HEIGHT = 1024;
const unsigned int SCR_WIDTH = SCR_HEIGHT * 16 / 9;

glm::vec2 posPrincipal(0.0f, 0.0f);
Sprite sprite1;

int main()
{
    std::cout << "Window size: (" << SCR_WIDTH << "," << SCR_HEIGHT << ")" << std::endl;

    // glfw: initialize and configure
    // ------------------------------
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    Shader shader("./shaders/simple.vert", "./shaders/simple.frag");
    // Shader shader("./shaders/simple.vert", "./shaders/reflection.frag");
    shader.use();

    // glm::mat4 projection(1.0f);
    // glm::mat4 projection = glm::ortho(0.0f, 40.0f, 0.0f, 40.0f, -1.0f, 1.0f);
    glm::mat4 projection = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT, -1.0f, 1.0f);
    shader.setMat4("projection", projection);

    Texture texture1;
    Texture texture2;
    Texture texBackground;
    texture1.load("./texturas/numeros.png");
    texture1.load("./texturas/goku.png");
    texture2.load("./texturas/sprite.png");
    texBackground.load("./texturas/background.jpg");

    Sprite spriteBackground;
    Sprite sprite2;

    float width = texture1.getWidth() / 4.0f;
    float height = texture1.getHeight() / 4.0f;
    glm::vec2 pos0((SCR_WIDTH - width) / 2.0, (SCR_HEIGHT - height) / 2.0);

    sprite1.translate(pos0.x, pos0.y);
    sprite1.scale(width, height);

    sprite2.translate(pos0.x, pos0.y);
    sprite2.scale(texture2.getWidth() / 4.0f, texture2.getHeight() / 2.0f);

    class A : public Animation {
        private:
        Sprite *sprite;
        glm::vec3 startLocation;
        float startTimeUpdate = -1.0f;

        public:
        A(Sprite *sprite) {
            this->sprite = sprite;
            this->startLocation = sprite->getTranslateVec();
        }

        virtual bool update(float currentTime) {
            if(startTimeUpdate == -1.0f) {
                startTimeUpdate = currentTime;
            }
            float ellapsedTime = currentTime - startTimeUpdate;

            float x = this->startLocation.x + (ellapsedTime * 1000.0f);
            float y = this->startLocation.y;
            sprite->translate(x, y);

            // return true; //->animacao infinita
            // return (this->startLocation.x + (ellapsedTime * 100.0f)) < 1600.0f;
            return ellapsedTime < 1.0f;
        };

    } anim1(&sprite1);
    sprite1.animation = &anim1;

    class B : public Animation
    {
      private:
        Sprite *sprite;
        glm::vec3 startLocation;
        float startTimeUpdate = -1.0f;

      public:
        B(Sprite *sprite)
        {
            this->sprite = sprite;
            this->startLocation = sprite->getTranslateVec();
        }

        virtual bool update(float currentTime)
        {
            if (startTimeUpdate == -1.0f)
            {
                startTimeUpdate = currentTime;
            }
            float ellapsedTime = currentTime - startTimeUpdate;


            // float x = this->startLocation.x + (ellapsedTime * 50.0f);
            if(ellapsedTime > 0.05f) {
                startTimeUpdate = currentTime;
                sprite->nextFrame();
            }

            return true; //->animacao infinita
            // return (this->startLocation.x + (ellapsedTime * 100.0f)) < 1600.0f;
            // return ellapsedTime < 1.0f;
        };

    } anim2(&sprite2);

    sprite2.animation = &anim2;

    spriteBackground.translate(0, 0);
    spriteBackground.scale(SCR_WIDTH, SCR_HEIGHT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    sprite1.setTexture(&texture1);
    sprite2.setTexture(&texture2, 2, 4);
    spriteBackground.setTexture(&texBackground);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        // glClearColor(0.02f, 0.05f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        float currentTime = (float)glfwGetTime();

        spriteBackground.update(currentTime);
        spriteBackground.draw(&shader);

        sprite1.update(currentTime);
        sprite1.draw(&shader);

        sprite2.update(currentTime);
        sprite2.draw(&shader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        GLenum error = glGetError();
        if(error != 0) {
          std::cout << "erro no opengl: " << error << std::endl;
        }
    }

    sprite1.gc();
    sprite2.gc();
    // spriteBackground.gc();
    texture1.free();
    texture2.free();
    texBackground.free();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
