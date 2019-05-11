#include "app.hpp"
#include "include/mapbox/earcut.hpp"
#include "include/delfrrr/delaunator.hpp"
#include "engine/text/Glyph.hpp"

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
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

using Coord = float;
using N = uint32_t;
using Point = std::array<Coord, 2>;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -3050.0f);
glm::vec3 lightPos  = glm::vec3(500.0f, 1000.0f, 1000.0f);

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
    Glyph glyph(face, 'E');
    glyph.parse();
    // glyph.printTest();

    srand(static_cast<unsigned>(time(0)));

    std::vector<std::vector<glm::vec2>> *contours = glyph.getContours();
    std::vector<std::vector<Point>> contoursPoints2d;
    std::vector<glm::vec3> contoursPoints3d;
    // std::vector<glm::vec3> sideFaces;

    float zValue = 0.0f;
    for(auto it = contours->begin(); it != contours->end(); it++) {
        std::vector<Point> points2d;

        for(size_t i=0; i< it->size(); i++) {
            // glm::vec3 point = glm::vec3(it->at(i), zValue);
            // glm::vec3 nextPoint = glm::vec3(it->at(i + 1 == it->size() ? 0 : i + 1), zValue);

            // contoursPoints3d.push_back(point);
            // contoursPoints3d.push_back({0.0f, 0.0f, 1.0f});

            points2d.push_back({it->at(i).x, it->at(i).y});
        }

        contoursPoints2d.push_back(points2d);
    }

    std::vector<std::vector<Point>> polygon;
    polygon.push_back(contoursPoints2d.at(0));

    std::vector<N> indices = mapbox::earcut<N>(polygon);

    for(size_t i=0; i<indices.size(); i++) {
        N index = indices[i];
        Point *p = &contoursPoints2d.at(0).at(index);

        contoursPoints3d.push_back(glm::vec3(std::get<0>(*p), std::get<1>(*p), zValue));
        contoursPoints3d.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    }

    for(size_t i=0; i<indices.size(); i++) {
        N index = indices[i];
        Point *p = &contoursPoints2d.at(0).at(index);

        contoursPoints3d.push_back(glm::vec3(std::get<0>(*p), std::get<1>(*p), zValue - 100.0f));
        contoursPoints3d.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    }
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

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.0f, 1000.0f);

    unsigned int /*EBO, */frontVBO, frontVAO;
    glGenVertexArrays(1, &frontVAO);
    glGenBuffers(1, &frontVBO);
    // glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(frontVAO);

    float *_contoursPoints = reinterpret_cast<float *>(contoursPoints3d.data());
    glBindBuffer(GL_ARRAY_BUFFER, frontVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * contoursPoints3d.size() * 3, _contoursPoints, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(N) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //-------------------------------------------
    
    //-------------------------------------------

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
        glm::mat4 model(1.0f);
        glm::mat4 view(1.0f);

        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
        // model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::translate(view, cameraPos);

        shader.use();
        shader.setVec3("uColor", 0.3f, 0.7f, 0.9f);
        shader.setVec3("uLightColor", 1.0f, 1.0f, 1.0f);
        shader.setVec3("uLightPos", lightPos);
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setFloat("animation", sin(glfwGetTime()));

        // glBindVertexArray(sVAO);
        // glDrawArrays(GL_TRIANGLE_STRIP, 0, contours->at(0).size() << 1);

        glBindVertexArray(frontVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

        // glMultiDrawElements(GL_TRIANGLES, indices_count.data(), GL_UNSIGNED_INT, elem_values, indices.size());
        // glDrawElements(GL_TRIANGLES, indices.size() * sizeof(N), GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, contours->at(0).size() * 6);

        glBindVertexArray(0); // no need to unbind it every time

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
        // glCheckError();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &frontVAO);
    // glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &frontVBO);

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
