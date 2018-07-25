#include "app.hpp"
#include "engine/Sprite.hpp"
#include "engine/Texture.hpp"
#include "engine/Animation.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_HEIGHT = 1024;
const unsigned int SCR_WIDTH = SCR_HEIGHT * 16 / 9;

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


    float lastTime = (float)glfwGetTime();
    Texture texture1;
    texture1.load("./texturas/goku.png");
    // texture1.load("./texturas/sprite.png");
    Sprite sprite1(2, 4, &texture1);

    float width  = texture1.getWidth()  / 8.0;
    float height = texture1.getHeight() / 8.0;
    // sprite1.translate((SCR_WIDTH - width) / 2.0, (SCR_HEIGHT - height) / 2.0);
    sprite1.translate(100.0f, (SCR_HEIGHT - height) / 2.0);
    sprite1.scale(width, height);

    Animation animation(&sprite1);
    // animation.move(lastTime, 10.0f, 800.0f, 0.0f);

    // glm::mat4 matmodel(1.0);
    animation.setup(lastTime, 2.0f, 
        [&sprite1, width, height](glm::vec3 startLocation, glm::vec3 startScale, float startAngle, float percent) {

        // static float dx = 1000.0f;
        sprite1.translate((SCR_WIDTH - width) / 2.0 * percent, (SCR_HEIGHT - height) / 2.0 * percent);
        sprite1.scale(width * percent, height * percent);
    });

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

        animation.update(currentTime);
        sprite1.draw(&shader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        // GLenum error = glGetError();
        // if(error != 0) {
        //   std::cout << "erro no opengl" << '\n';;
        // }
    }

    sprite1.gc();

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
