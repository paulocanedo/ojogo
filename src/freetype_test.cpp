#include "app.hpp"
#include "include/mapbox/earcut.hpp"
#include "include/delfrrr/delaunator.hpp"
#include "engine/text/Glyph.hpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

using Coord = float;
using N = uint32_t;
using Point = std::array<Coord, 2>;

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

    // For simplicity, use the charmap FreeType provides by default;
    // in most cases this means Unicode.
    FT_Face face;
    FT_New_Face(ft_library, "/home/paulocanedo/Downloads/fonts/Ubuntu-R.ttf", 0, &face);
    //   FT_New_Face(ft_library, "/home/paulocanedo/Downloads/fonts/Txt Regular.ttf", 0, &face);
    //   FT_New_Face(ft_library, "/home/paulocanedo/Downloads/fonts/Air Millhouse Outline.ttf", 0, &face);
    //   FT_New_Face(ft_library, "/home/paulocanedo/Downloads/fonts/Camouflage Snow Snow.ttf", 0, &face);
    //   FT_New_Face(ft_library, "/home/paulocanedo/Downloads/fonts/Starcraft Normal.ttf", 0, &face);
    //   FT_New_Face(ft_library, "/home/paulocanedo/Downloads/fonts/wetp.ttf", 0, &face);
    Glyph glyph(face, '@');
    glyph.parse();
    glyph.printTest();

    std::vector<std::vector<glm::vec2>> *contours = glyph.getContours();
    std::vector<Point> contoursPoints;
    std::vector<int> contoursPointsFirst;
    std::vector<int> contoursPointsCount;

    for(auto it = contours->begin(); it != contours->end(); it++) {
        contoursPointsFirst.push_back(contoursPoints.size());
        contoursPointsCount.push_back(it->size());

        // contoursPoints.insert(contoursPoints.end(), it->begin(), it->end());
        
        for(auto it2 = it->begin(); it2 != it->end(); it2++) {
            contoursPoints.push_back({it2->x, it2->y});
        }
    }
    float *_contoursPoints = reinterpret_cast<float *>(contoursPoints.data());
    int *_contoursPointsFirst = reinterpret_cast<int *>(contoursPointsFirst.data());
    int *_contoursPointsCount = reinterpret_cast<int *>(contoursPointsCount.data());

    std::vector<std::vector<Point>> polygon;
    // contoursPoints.push_back(contoursPoints.at(0));
    polygon.push_back(contoursPoints);

    std::cout << "==========================================" << std::endl;
    for(auto it = contoursPoints.begin(); it != contoursPoints.end(); it++) {
        std::cout << it->at(0);
        std::cout << ",";
        std::cout << it->at(1);
        std::cout << std::endl;
    }
    std::cout << "==========================================" << std::endl;
    
    std::vector<N> indices = mapbox::earcut<N>(polygon);
    // printIndices(indices);
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

    // glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    Shader shader("./shaders/simple.vert", "./shaders/simple.frag");
    glm::mat4 model(1.0f);
    // glm::mat4 projection = glm::ortho(xMin - 100, xMax + 100, yMin - 100, yMax + 100, -1.0f, 1.0f);
    glm::mat4 projection = glm::ortho(-500.0f, 2000.0f, -500.0f, 2000.0f, -1.0f, 1.0f);

    // glm::mat4 model = glm::mat4(1.0f);
    // glm::mat4 view = glm::mat4(1.0f);
    // glm::mat4 projection = glm::mat4(1.0f);

    // model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
    // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    // projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    unsigned int EBO, VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * contoursPoints.size() * 2, _contoursPoints, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(N) * indices.size(), indices.data(), GL_STATIC_DRAW);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(size_t) * triangles->size() * 3, triangles, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

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

        // draw our first triangle
        // glUseProgram(shaderProgram);
        shader.use();
        shader.setMat4("model", model);
        shader.setMat4("projection", projection);

        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

        shader.setVec3("uColor", 0.3f, 0.7f, 0.9f);
        // glMultiDrawElements(GL_TRIANGLES, indices_count.data(), GL_UNSIGNED_INT, elem_values, indices.size());
        glDrawElements(GL_TRIANGLES, indices.size() * sizeof(N), GL_UNSIGNED_INT, 0);
        // glDrawElements(GL_TRIANGLES, triangles->size() * 3 * sizeof(size_t), GL_UNSIGNED_INT, 0);

        shader.setVec3("uColor", 0.0f, 0.0f, 0.0f);
        glMultiDrawArrays(GL_LINE_LOOP, _contoursPointsFirst, _contoursPointsCount, static_cast<int>(contours->size()));

        glBindVertexArray(0); // no need to unbind it every time

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
