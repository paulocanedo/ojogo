#include "app.hpp"
#include "include/mapbox/earcut.hpp"
#include "include/delfrrr/delaunator.hpp"
#include "engine/text/Glyph.hpp"
#include "engine/text/TextTessellation.hpp"

GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
        case GL_INVALID_ENUM:
            error = "INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            error = "INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            error = "INVALID_OPERATION";
            break;
        case GL_STACK_OVERFLOW:
            error = "STACK_OVERFLOW";
            break;
        case GL_STACK_UNDERFLOW:
            error = "STACK_UNDERFLOW";
            break;
        case GL_OUT_OF_MEMORY:
            error = "OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error = "INVALID_FRAMEBUFFER_OPERATION";
            break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH  = 800 * 1;
const unsigned int SCR_HEIGHT = 600 * 1;

// using Coord = float;
// using N = uint32_t;
// using Point = std::array<Coord, 2>;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -3000.0f);
glm::vec3 lightPos  = glm::vec3(500.0f, 1000.0f, 1000.0f);

int main()
{
    FT_Library ft_library;
    FT_Error error = FT_Init_FreeType(&ft_library);
    if (error)
    {
        std::cout << "Failed loading freetype lib" << std::endl;
        return -1;
    }

    // For simplicity, use the charmap FreeType provides by default;
    // in most cases this means Unicode.
    FT_Face face;
    FT_New_Face(ft_library, "/home/paulocanedo/Downloads/fonts/Ubuntu-R.ttf", 0, &face);
    //   FT_New_Face(ft_library, "/home/paulocanedo/Downloads/fonts/Txt Regular.ttf", 0, &face);
    //   FT_New_Face(ft_library, "/home/paulocanedo/Downloads/fonts/Air Millhouse Outline.ttf", 0, &face);
    //   FT_New_Face(ft_library, "/home/paulocanedo/Downloads/fonts/Camouflage Snow Snow.ttf", 0, &face);
    //   FT_New_Face(ft_library, "/home/paulocanedo/Downloads/fonts/Starcraft Normal.ttf", 0, &face);
    //   FT_New_Face(ft_library, "/home/paulocanedo/Downloads/fonts/wetpm.ttf", 0, &face);

    std::string text = "paulo?";
    // std::string text = "Paulo";
    std::vector<Glyph> visualText;

    for(auto it = text.begin(); it != text.end(); it++) {
        unsigned long character = *it;
        Glyph glyph(face, character);
        visualText.push_back(glyph);
    }

    // Glyph glyph(face, 'G');
    // Glyph glyph2(face, 'H');

    srand(static_cast<unsigned>(time(0)));

    // const std::vector<std::vector<glm::vec2>> *contours = glyph.getContours();

    // TextTessellation tt;
    // std::vector<glm::vec3> contoursPoints3d = tt.tessellate(*contours, 100.0f);
    //--------------------------------------------------------------

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    Shader shader("./shaders/simple.vert", "./shaders/simple.frag");

    // glm::mat4 projection = glm::ortho(-1000.0f, 1000.0f, -1000.0f, 1000.0f, -1000.0f, 1000.0f);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, -100.0f, 1000.0f);

    for(auto it = visualText.begin(); it != visualText.end(); it++)
    {
        it->gpuUpload();
    }
    // glyph.gpuUpload();
    // glyph2.gpuUpload();


    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model(1.0f);
        glm::mat4 view(1.0f);

        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::translate(view, cameraPos);

        shader.use();
        shader.setVec3("uColor", 0.3f, 0.7f, 0.9f);
        shader.setVec3("uLightColor", 1.0f, 1.0f, 1.0f);
        shader.setVec3("uLightPos", lightPos);
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setFloat("animation", sin(glfwGetTime()));

        for (auto it = visualText.begin(); it != visualText.end(); it++)
        {    
            shader.setMat4("model", model);
            it->gpuDraw();
            model = glm::translate(model, glm::vec3(800.0f, 0.0f, 0.0f));
        }


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
        // glCheckError();
    }

    for (auto it = visualText.begin(); it != visualText.end(); it++)
    {
        it->gpuFreeResources();
    }

    FT_Done_FreeType(ft_library);
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 10.0f;

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        cameraPos.x -= cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        cameraPos.x += cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cameraPos.y += cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cameraPos.y -= cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
        cameraPos.z += cameraSpeed * 5.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
        cameraPos.z -= cameraSpeed * 5.0f;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
