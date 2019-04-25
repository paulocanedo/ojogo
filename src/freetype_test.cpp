#include "app.hpp"
#include "include/mapbox/earcut.hpp"

#define BEZIER_STEP 0.01f

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

static int MoveToFunction(FT_Vector *to, void *user);
static int LineToFunction(FT_Vector *to, void *user);
static int ConicToFunction(FT_Vector *control, FT_Vector *to, void *user);
static int CubicToFunction(const FT_Vector *controlOne,
                           const FT_Vector *controlTwo,
                           const FT_Vector *to,
                           void *user);

using Coord = float;
using N = uint32_t;
using Point = std::array<Coord, 2>;

struct char_drawing_t
{
    int control = 0;
    std::vector<int> first;
    std::vector<int> count;
    std::vector<Point> data;
} char_drawing;

int MoveToFunction(FT_Vector *to, void *user)
{
    char_drawing_t *info = static_cast<char_drawing_t *>(user);

    if (info->control > 0)
    {
        info->count.push_back(info->control - info->first.back());
    }
    info->first.push_back(info->control);

    float x = to->x, y = to->y;
    info->data.push_back({x, y});
    info->control++;

    return 0;
}

int LineToFunction(FT_Vector *to, void *user)
{
    char_drawing_t *info = static_cast<char_drawing_t *>(user);
    float x = to->x, y = to->y;
    info->data.push_back({x, y});
    info->control++;

    return 0;
}

int ConicToFunction(FT_Vector *control, FT_Vector *to, void *user)
{
    char_drawing_t *info = static_cast<char_drawing_t *>(user);

    Point p0 = info->data.end()[-1];
    float px0 = p0[0];
    float py0 = p0[1];

    float px1 = control->x;
    float py1 = control->y;

    float px2 = to->x;
    float py2 = to->y;
    for (float delta = 0.0f; delta <= 1.0f; delta += BEZIER_STEP)
    {
        float x = pow(1 - delta, 2) * px0 + (1 - delta) * 2 * delta * px1 + delta * delta * px2;
        float y = pow(1 - delta, 2) * py0 + (1 - delta) * 2 * delta * py1 + delta * delta * py2;

        info->data.push_back({x, y});
        info->control++;
    }

    return 0;
}

static int CubicToFunction(const FT_Vector *controlOne,
                           const FT_Vector *controlTwo,
                           const FT_Vector *to,
                           void *user)
{

    char_drawing_t *info = static_cast<char_drawing_t *>(user);

    Point p = info->data.end()[-1];
    float px0 = p[0];
    float py0 = p[1];

    float px1 = controlOne->x;
    float py1 = controlOne->y;

    float px2 = controlTwo->x;
    float py2 = controlTwo->y;

    float px3 = to->x;
    float py3 = to->y;
    for (float delta = 0.0f; delta <= 1.0f; delta += BEZIER_STEP)
    {
        float x = pow(1 - delta, 3) * px0 + pow(1 - delta, 2) * 3 * delta * px1 + (1 - delta) * 3 * delta * delta * px2 + delta * delta * delta * px3;
        float y = pow(1 - delta, 3) * py0 + pow(1 - delta, 2) * 3 * delta * py1 + (1 - delta) * 3 * delta * delta * py2 + delta * delta * delta * py3;

        info->data.push_back({x, y});
        info->control++;
    }

    return 0;
}

void printPolygon(const std::vector<Point> &polygon, const int start, const int count) {
    const int end = start + count;
    std::cout << std::endl;    
    for(int i=start, count = 0; i < end; i++, count++) {
        Point elem = polygon.at(i);
        std::cout << "(";
        std::cout << elem[0];
        std::cout << ',';
        std::cout << elem[1];
        std::cout << ");";  
    }
    std::cout << std::endl;    
}

void printIndices(const std::vector<N> &indices) {
    for(const N &value : indices) {
        std::cout << value;
        std::cout << ",";
    }
    std::cout << std::endl;
}

int main()
{
    FT_Library ft_library;
    FT_Error error = FT_Init_FreeType(&ft_library);
    if (error)
    {
        std::cout << "Failed loading freetype lib" << std::endl;
        return -1;
    }

    FT_ULong code = 'D';

    // For simplicity, use the charmap FreeType provides by default;
    // in most cases this means Unicode.
    FT_Face face;
    FT_New_Face(ft_library, "/home/paulocanedo/Downloads/fonts/Ubuntu-R.ttf", 0, &face);
    //   FT_New_Face(ft_library, "/home/paulocanedo/Downloads/fonts/Txt Regular.ttf", 0, &face);
    //   FT_New_Face(ft_library, "/home/paulocanedo/Downloads/fonts/Air Millhouse Outline.ttf", 0, &face);
    //   FT_New_Face(ft_library, "/home/paulocanedo/Downloads/fonts/Camouflage Snow Snow.ttf", 0, &face);
    //   FT_New_Face(ft_library, "/home/paulocanedo/Downloads/fonts/Starcraft Normal.ttf", 0, &face);
    //   FT_New_Face(ft_library, "/home/paulocanedo/Downloads/fonts/wetp.ttf", 0, &face);

    FT_UInt index = FT_Get_Char_Index(face, code);

    error = FT_Load_Glyph(face,
                          index,
                          FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP);

    if (error)
    {
        std::cout << "Couldn't load the glyph: FT_Load_Glyph() failed" << std::endl;
        return -1;
    }

    FT_Outline_Funcs callbacks;

    callbacks.move_to = (FT_Outline_MoveTo_Func)&MoveToFunction;
    callbacks.line_to = (FT_Outline_LineTo_Func)&LineToFunction;
    callbacks.conic_to = (FT_Outline_ConicTo_Func)&ConicToFunction;
    callbacks.cubic_to = (FT_Outline_CubicTo_Func)&CubicToFunction;

    callbacks.shift = 0;
    callbacks.delta = 0;

    FT_GlyphSlot slot = face->glyph;
    FT_Outline &outline = slot->outline;

    error = FT_Outline_Decompose(&outline, &callbacks, &char_drawing);
    char_drawing.count.push_back(char_drawing.control - char_drawing.first.back());

    if (error)
    {
        std::cout << "Couldn't extract the outline: FT_Outline_Decompose() failed";
        return -1;
    }

    FT_BBox boundingBox;
    FT_Outline_Get_BBox(&outline, &boundingBox);

    float xMin = boundingBox.xMin;
    float yMin = boundingBox.yMin;
    float xMax = boundingBox.xMax;
    float yMax = boundingBox.yMax;

    std::vector<std::vector<Point>> polygon1;
    int start1 = char_drawing.first[0];
    int count1 = char_drawing.count[0];

    auto first1 = char_drawing.data.cbegin() + start1;
    auto last1 = char_drawing.data.cbegin() + (start1 + count1);
    std::vector<Point>p1(first1, last1);
    polygon1.push_back(p1);
    std::vector<N> indices1 = mapbox::earcut<N>(polygon1);

    printPolygon(char_drawing.data, start1, count1);
    printIndices(indices1);

    //--------------------------------------------------------------
    std::vector<std::vector<Point>> polygon2;
    int start2 = char_drawing.first[1];
    int count2 = char_drawing.count[1];

    auto first2 = char_drawing.data.cbegin() + start2;
    auto last2 = char_drawing.data.cbegin() + (start2 + count2);
    std::vector<Point>p2(first2, last2);
    polygon2.push_back(p2);
    std::vector<N> indices2 = mapbox::earcut<N>(polygon2);

    printPolygon(char_drawing.data, start1, count1);
    printIndices(indices2);

    //--------------------------------------------------------------
    std::vector<std::vector<Point>> polygon;
    polygon.push_back(p1);
    polygon.push_back(p2);
    std::vector<N> indicesAll = mapbox::earcut<N>(polygon);


    std::vector<N> indices = indicesAll;
    printIndices(indicesAll);


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

    glEnable(GL_MULTISAMPLE);
    Shader shader("./shaders/simple.vert", "./shaders/simple.frag");
    glm::mat4 model(1.0f);
    glm::mat4 projection = glm::ortho(xMin - 100, xMax + 100, yMin - 100, yMax + 100, -1.0f, 1.0f);
    // glm::mat4 projection = glm::ortho(-500.0f, 2000.0f, -500.0f, 2000.0f, -1.0f, 1.0f);

    std::cout << "char: " << (char)code << std::endl;

    unsigned int EBO, VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    float *values = reinterpret_cast<float *>(char_drawing.data.data());
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * char_drawing.data.size() * 2, values, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    int glMajor, glMinor, glRef;
    glfwGetVersion(&glMajor, &glMinor, &glRef);

    std::cout << "GL Version: " << glMajor << "." << glMinor << " rev-" << glRef << std::endl;

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
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        // glUseProgram(shaderProgram);
        shader.use();
        shader.setMat4("model", model);
        shader.setMat4("projection", projection);

        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

        shader.setVec3("uColor", 0.3f, 0.7f, 0.9f);
        // glMultiDrawElements(GL_TRIANGLES, indices_count.data(), GL_UNSIGNED_INT, elem_values, indices.size());
        glDrawElements(GL_TRIANGLES, sizeof(N) * indices.size(), GL_UNSIGNED_INT, 0);

        // shader.setVec3("uColor", 0.0f, 0.0f, 0.0f);
        // glMultiDrawArrays(GL_LINE_LOOP, char_drawing.first.data(), char_drawing.count.data(), char_drawing.control);

        // glBindVertexArray(0); // no need to unbind it every time

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();

    FT_Done_FreeType(ft_library);
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
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
