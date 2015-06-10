#include <iostream>
#include <string>
#include <sstream>

#include "utility/Ein.hpp"

// Other includes
#include "utility/Shader.hpp"
#include "utility/Texture.hpp"
#include "utility/glfwUtils.hpp"
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Window dimensions
const GLuint WIDTH = 1600, HEIGHT = 1200;

void onError(int error, const char *description);
void showFPS(EinWindow* pWindow);

int main()
{
    EinApplication *app = new EinApplication();
    app->setErrorCallback(onError);

    // EinWindow window(WIDTH, HEIGHT, "cgBentRendering", false);

    EinWindow* window = new EinWindow(WIDTH, HEIGHT, "cgBentRendering", true);
    // Setup OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers(1, &vbo);

    // Set up vertex data (and buffer(s)) and attribute pointers
    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // GLfloat vertices[] = {
    //     // Positions          // Colors           // Texture Coords
    //     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
    //     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
    //     -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
    //     -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left
    // };

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // // Create a element array buffer object
    // GLuint ebo;
    // glGenBuffers(1, &ebo);
    //
    // GLuint indices[] = {  // Note that we start from 0!
    //     0, 1, 3, // First Triangle
    //     1, 2, 3  // Second Triangle
    // };
    //
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    Shader ourShader("shaders/base.vert", "shaders/base.frag");
    glBindFragDataLocation(ourShader.Program, 0, "outColor");
    ourShader.Link();

    // Specify the layout of the vertex data
    // Vertex coordinates
    GLint posAttrib = glGetAttribLocation(ourShader.Program, "position");
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid *) 0);
    glEnableVertexAttribArray(posAttrib);

    // // Vertex colors
    // GLint colorAttrib = glGetAttribLocation(ourShader.Program, "color");
    // glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid *)(3 * sizeof(GL_FLOAT)));
    // glEnableVertexAttribArray(colorAttrib);

    // Texture coordinates
    GLint texPosAttrib = glGetAttribLocation(ourShader.Program, "texPosition");
    glVertexAttribPointer(texPosAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid *)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(texPosAttrib);

    // Unbind the vao
    glBindVertexArray(0);

    // Load and create a texture
    Texture firstTexture("textures/firstTexture.png", GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);

    while(!window->ShouldClose())
    {
        window->GetInputManager()->poolEvents();
        if((window->GetInputManager())->isUp(EinInputManager::KeyActionType::KEY_UP)){
            window->SetShouldClose(true);
        }
        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind texture
        glActiveTexture(GL_TEXTURE0);
        firstTexture.Bind();
        glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture"), 0);

        //  Activate shader
        ourShader.Use();

        // Create transformations
        glm::mat4 view;
        glm::mat4 projection;
        glm::mat4 model;
        model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));
        projection = glm::perspective(glm::radians(45.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
        // Get their uniform location
        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // Draw vertexes
        glBindVertexArray(vao);
        // glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));
        model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));
        glBindVertexArray(0);

        // Swap screen buffer
        window->SwapBuffers();
        showFPS(window);
    }

    // Clean up resources
    ourShader.Delete();

    glDeleteBuffers(1, &vbo);
    // glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
    delete window;
    delete app;
}

void onError(int error, const char *description) {
    std::cerr << "[APPLICATION] Error:" << error << "description: " << description << std::endl;
}

void showFPS(EinWindow* pWindow)
{
    // Measure speed
    static double lastTime;
    static int nbFrames;
    double currentTime = glfwGetTime();
    double delta = currentTime - lastTime;
    nbFrames++;
    if ( delta >= 1.0 ){ // If last cout was more than 1 sec ago
        // std::cout << 1000.0/double(nbFrames) << std::endl;

        double fps = double(nbFrames) / delta;

        std::stringstream ss;
        ss << "cgBentRendering [" << fps << " FPS]";

        pWindow->SetTitle(ss.str());

        nbFrames = 0;
        lastTime = currentTime;
    }
}
