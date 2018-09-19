#include "app.hpp"
#include "engine/Game.hpp"
#include "engine/Sprite.hpp"

#include "engine/animation/AnimationMultiImage.hpp"
#include "engine/animation/AnimationTranslate.hpp"
#include "engine/animation/AnimationQuedaLivre.hpp"
#include "engine/animation/AnimationBezier.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_HEIGHT = 1024;
const unsigned int SCR_WIDTH = SCR_HEIGHT * 16 / 9;

std::shared_ptr<Sprite> goku;

int main()
{
    
    std::cout << "Window size: (" << SCR_WIDTH << "," << SCR_HEIGHT << ")" << std::endl;

    Game* game = new Game();
    game->setup();    
    // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // build and compile our shader program
    // ------------------------------------
    Shader shader("./shaders/simple.vert", "./shaders/simple.frag");

    goku = Sprite::fromTexture("./texturas/goku.png", 200.0f);
    goku->translate(20, 20);

    // std::shared_ptr<Sprite> bola = Sprite::fromTexture("./texturas/awesomeface.png", 32.0f);
    // bola->translate(700, SCR_HEIGHT - 30);

    // std::shared_ptr<Sprite> cat = Sprite::fromMultiImage("./texturas/sprite.png", 200.0f, 2, 4);
    // cat->translate(SCR_WIDTH - 300, 20);

    // std::unique_ptr<AnimationMultiImage> amt = std::make_unique<AnimationMultiImage>();
    // cat->animations.push_back(amt.get());

     std::unique_ptr<AnimationBezier> at = std::make_unique<AnimationBezier>(glm::vec2(0.0f, 0.0f),glm::vec2(1000.0f, 350.0f),glm::vec2(0.0f, 700.0f),glm::vec2(500.0f, 0.0f),10);
     goku->animations.push_back(at.get());

    glm::mat4 projection = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT, -1.0f, 1.0f);

    game->setShader(&shader);
    game->setWorldMatrix(projection);

    game->add(goku.get());
    game->start();

    delete game;

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();

    std::cin.get();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS) return;

    float duration = 0.5f;
    float distance = 300.0f;
    bool right = true;
    bool flag = false;

    switch (key)
    {
    case GLFW_KEY_RIGHT:
        right = true;
        flag = true;
        std::cout << "pressed key: right, " << action << std::endl;
        break;
    case GLFW_KEY_DOWN:
        std::cout << "pressed key: down, " << action << std::endl;
        break;
    case GLFW_KEY_LEFT:
        right = false;
        flag = true;
        std::cout << "pressed key: left, " << action << std::endl;
        break;
    case GLFW_KEY_UP:
        std::cout << "pressed key: up, " << action << std::endl;
        break;
    default:
        break;
    }

    if(flag) {
        goku->animations.push_back(new AnimationTranslate(duration, distance * (right ? 1.0f : -1.0f), 0.0f));
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// int MoveToFunction(const FT_Vector *to,
//                    void *user);
// int LineToFunction(const FT_Vector *to,
//                    void *user);
// int ConicToFunction(const FT_Vector *control,
//                     const FT_Vector *to,
//                     void *user);
// int CubicToFunction(const FT_Vector *controlOne,
//                     const FT_Vector *controlTwo,
//                     const FT_Vector *to,
//                     void *user);

// int MoveToFunction(const FT_Vector *to, void *user)
// {
//     std::cout << "moveTo: " << to->x << "," << to->y << std::endl;
//     return 0;
// }

// int LineToFunction(const FT_Vector *to, void *user)
// {
//     return 0;
// }

// int ConicToFunction(const FT_Vector *control, const FT_Vector *to, void *user)
// {
//     return 0;
// }

// int CubicToFunction(const FT_Vector *controlOne,
//                     const FT_Vector *controlTwo,
//                     const FT_Vector *to,
//                     void *user)
// {

//     return 0;
// }

// //experimental render text
// FT_Library m_ftLibrary;
// FT_Error error = FT_Init_FreeType(&m_ftLibrary);
// if (error)
// {
//     std::cerr << "falha ao carregar freetype lib" << std::endl;
//     return 2;
// }

// FT_Face m_ftFace;
// error = FT_New_Face(m_ftLibrary, "/home/paulocanedo/Downloads/LucidaSansRegular.ttf", 0, &m_ftFace);
// if (error)
// {
//     std::cerr << "falha ao carregar fonte..." << std::endl;
//  {
// }{

// F{ = 'P';
// F{ = FT_Get_Char_Index(m_ftFace, code);

// e{ad_Glyph(m_ftFace,
//  {         index,
//  {         FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP);

// i{
// {{
//     std::cerr << "falha ao carregar glyph..." << std::endl;
//     return 3;
// }

// FT_Outline_Funcs callbacks;
// callbacks.move_to = MoveToFunction;
// callbacks.line_to = LineToFunction;
// callbacks.conic_to = ConicToFunction;
// callbacks.cubic_to = CubicToFunction;

// callbacks.shift = 0;
// callbacks.delta = 0;

// FT_GlyphSlot slot = m_ftFace->glyph;
// FT_Outline &outline = slot->outline;

// error = FT_Outline_Decompose(&outline, &callbacks, NULL);
// // render text

// FT_Done_FreeType(m_ftLibrary);