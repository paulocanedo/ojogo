#include "app.hpp"
#include "engine/Sprite.hpp"
#include "engine/Texture.hpp"
#include "engine/TransformAnimation.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_HEIGHT = 1024 * 2;
const unsigned int SCR_WIDTH = SCR_HEIGHT * 16 / 9;

int main()
{
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
    // Shader shader("../shaders/simple.vert", "../shaders/simple.frag");
    Shader shader("../shaders/simple.vert", "../shaders/reflection.frag");
    shader.use();

    // glm::mat4 projection(1.0f);
    // glm::mat4 projection = glm::ortho(0.0f, 40.0f, 0.0f, 40.0f, -1.0f, 1.0f);
    glm::mat4 projection = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT, -1.0f, 1.0f);
    shader.setMat4("projection", projection);


    float lastTime = (float)glfwGetTime();
    Texture texture1;
    texture1.load("../texturas/goku.png");
    // texture1.load("./texturas/sprite.png");
    Sprite sprite1(2, 4, &texture1);

    float width  = texture1.getWidth()  / 2.0;
    float height = texture1.getHeight() / 2.0;
    sprite1.translate((SCR_WIDTH - width) / 2.0, (SCR_HEIGHT - height) / 2.0);
    sprite1.scale(width, height);

    TransformAnimation animation(&sprite1);
    animation.move(lastTime, 3.0f, 1600.0f, 0.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // sprite1.move(time, 5.0f, 1000.0, 100.0f);

    int i=0, j=0;

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

        if(currentTime - lastTime > 0.15) {
          i++;j++;
          if(i >= 2) i =0;
          if(j >= 4) j =0;
          // sprite1.setCurrentElement(i, j);
          lastTime = currentTime;
        }

        animation.update(currentTime);
        sprite1.draw(&shader);

        // shader.setBool("reflected", true);
        // sprite2.draw(&shader);

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

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    // glDeleteVertexArrays(1, &VAO);
    // glDeleteBuffers(1, &VBO);

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
