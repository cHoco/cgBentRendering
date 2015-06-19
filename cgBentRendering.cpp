#include <iostream>
#include <string>
#include <sstream>

#include "utility/Ein.hpp"

// Other includes
#include "utility/Camera.hpp"
#include "utility/Shader.hpp"
#include "utility/Texture.hpp"
#include "utility/Model.hpp"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Window dimensions
const GLuint WIN_WIDTH = 1600, WIN_HEIGHT = 1200;
GLfloat lastX = 800, lastY = 600;
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

void onError(int error, const char *description);
void showFPS(EinWindow* pWindow);
void doMovement(EinInputManager* inputManager, Camera *camera);

int main()
{
    EinApplication::setErrorCallback(onError);
    EinApplication *app = new EinApplication();

    EinWindow* window = new EinWindow(WIN_WIDTH, WIN_HEIGHT, "cgBentRendering", false);

    Camera* mainCamera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

    // Setup OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Create Vertex Array Object
    GLuint cubesVao;
    glGenVertexArrays(1, &cubesVao);
    glBindVertexArray(cubesVao);

    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers(1, &vbo);

    // Set up vertex data (and buffer(s)) and attribute pointers
    // GLfloat vertices[] = {
    //     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    //      0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    //      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    //     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    //
    //     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    //      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    //      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    //     -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    //     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //
    //     -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //     -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //     -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //
    //      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //      0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //
    //     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //      0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    //      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    //      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    //     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //
    //     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    //      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //     -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    //     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    // };
    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    Shader ourShader("shaders/base.vert", "shaders/base.frag");
    glBindFragDataLocation(ourShader.Program, 0, "outColor");
    ourShader.Link();

    // Specify the layout of the vertex data
    // Vertex coordinates
    GLint posAttrib = glGetAttribLocation(ourShader.Program, "position");
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid *) 0);
    glEnableVertexAttribArray(posAttrib);

    // Normal vectors
    GLint texPosAttrib = glGetAttribLocation(ourShader.Program, "normal");
    glVertexAttribPointer(texPosAttrib, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid *)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(texPosAttrib);

    // Unbind the vao
    glBindVertexArray(0);

    GLuint lightVao;
    glGenVertexArrays(1, &lightVao);
    glBindVertexArray(lightVao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    Shader lightShader("shaders/light.vert", "shaders/light.frag");
    lightShader.Link();
    // Specify the layout of the vertex data
    // Vertex coordinates
    GLint lightPosAttrib = glGetAttribLocation(lightShader.Program, "position");
    glVertexAttribPointer(lightPosAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid *) 0);
    glEnableVertexAttribArray(lightPosAttrib);

    glBindVertexArray(0);



    // Load and create a texture
    Texture firstTexture("textures/firstTexture.png", GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);

    // Model testModel("models/nanosuit/nanosuit.obj");
    Model testModel("models/demon.obj");
    Shader modelShader("shaders/model_loader.vert", "shaders/model_loader.frag");
    modelShader.Link();

// Draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Point light positions
    glm::vec3 pointLightPositions[] = {
        glm::vec3(2.3f, -1.6f, -3.0f),
        glm::vec3(-1.7f, 0.9f, 1.0f)
    };
    while(!window->ShouldClose())
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        window->GetInputManager()->pollEvents();
        if((window->GetInputManager())->isExit(KeyActionType::KEY_UP)) {
            window->SetShouldClose(true);
        }
        doMovement(window->GetInputManager(), mainCamera);
        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // // Bind texture
        // glActiveTexture(GL_TEXTURE0);
        // firstTexture.Bind();
        // glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture"), 0);

        //  Activate shader
        ourShader.Use();

        GLint objectColorLoc = glGetUniformLocation(ourShader.Program, "objectColor");
        GLint lightColorLoc  = glGetUniformLocation(ourShader.Program, "lightColor");
        GLint lightPosLoc = glGetUniformLocation(ourShader.Program, "lightPos");
        GLint viewerPosLoc = glGetUniformLocation(ourShader.Program, "viewerPos");
        glUniform3f(viewerPosLoc, mainCamera->position.x, mainCamera->position.y, mainCamera->position.z);
        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
        glUniform3f(lightColorLoc,  1.0f, 0.5f, 1.0f);
        // Create transformations
        glm::mat4 view;
        view = mainCamera->GetViewMatrix();
        glm::mat4 projection;
        projection = glm::perspective(mainCamera->zoomQuantity, (float)WIN_WIDTH/(float)WIN_HEIGHT, 0.1f, 1000.0f);
        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(cubesVao);
        for(GLuint i = 0; i < 10; i++)
        {
            // Calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model;
            model = glm::translate(model, cubePositions[i]);
            GLfloat angle = 20.0f * i;
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);

        // Also draw the lamp object, again binding the appropriate shader
        lightShader.Use();
        // Get location objects for the matrices on the lamp shader (these could be different on a different shader)
        modelLoc = glGetUniformLocation(lightShader.Program, "model");
        viewLoc  = glGetUniformLocation(lightShader.Program, "view");
        projLoc  = glGetUniformLocation(lightShader.Program, "projection");
        // Set matrices
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glm::mat4 model;
        model = glm::mat4();
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        // Draw the light object (using light's vertex attributes)
        glBindVertexArray(lightVao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        modelShader.Use();   // <-- Don't forget this one!
        modelLoc = glGetUniformLocation(modelShader.Program, "model");
        viewLoc  = glGetUniformLocation(modelShader.Program, "view");
        projLoc  = glGetUniformLocation(modelShader.Program, "projection");
        // Set matrices
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        // Transformation matrices

        // Draw the loaded model
        model = glm::translate(model, glm::vec3(-5.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f)); // It's a bit too big for our scene, so scale it down
        glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        // Set the lighting uniforms
        glUniform3f(glGetUniformLocation(modelShader.Program, "viewPos"), mainCamera->position.x, mainCamera->position.y, mainCamera->position.z);
        // Point light 1
        glUniform3f(glGetUniformLocation(modelShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);		
        glUniform3f(glGetUniformLocation(modelShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);		
        glUniform3f(glGetUniformLocation(modelShader.Program, "pointLights[0].diffuse"), 1.0f, 1.0f, 1.0f); 
        glUniform3f(glGetUniformLocation(modelShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(modelShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(modelShader.Program, "pointLights[0].linear"), 0.009);
        glUniform1f(glGetUniformLocation(modelShader.Program, "pointLights[0].quadratic"), 0.0032);		
        // Point light 2
        glUniform3f(glGetUniformLocation(modelShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);		
        glUniform3f(glGetUniformLocation(modelShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);		
        glUniform3f(glGetUniformLocation(modelShader.Program, "pointLights[1].diffuse"), 1.0f, 1.0f, 1.0f); 
        glUniform3f(glGetUniformLocation(modelShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(modelShader.Program, "pointLights[1].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(modelShader.Program, "pointLights[1].linear"), 0.009);
        glUniform1f(glGetUniformLocation(modelShader.Program, "pointLights[1].quadratic"), 0.0032);	
        testModel.Draw(modelShader);

        // Swap screen buffer
        window->SwapBuffers();
        showFPS(window);
    }

    // Clean up resources
    ourShader.Delete();

    glDeleteBuffers(1, &vbo);
    // glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &cubesVao);
    glDeleteVertexArrays(1, &lightVao);
    delete window;
    delete mainCamera;
    delete app;
}

void doMovement(EinInputManager* inputManager, Camera *camera) {
    if(inputManager->isUp(KeyActionType::KEY_DOWN))
        camera->ProcessKeyboard(FORWARD, deltaTime);
    if(inputManager->isDown(KeyActionType::KEY_DOWN))
        camera->ProcessKeyboard(BACKWARD, deltaTime);
    if(inputManager->isLeft(KeyActionType::KEY_DOWN))
        camera->ProcessKeyboard(LEFT, deltaTime);
    if(inputManager->isRight(KeyActionType::KEY_DOWN))
        camera->ProcessKeyboard(RIGHT, deltaTime);

    double scrollY = inputManager->getScrollOffsetY();
    if (scrollY!=0)
        camera->ProcessMouseScroll(scrollY);

    if(inputManager->didCursorPositionChange()) {
        double xpos = inputManager->getCursorPosX();
        double ypos = inputManager->getCursorPosY();
        if(firstMouse)
        {
            lastX = xpos;

            lastY = ypos;
            firstMouse = false;
        }

        GLfloat xoffset = xpos - lastX;
        GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

        lastX = xpos;
        lastY = ypos;

        camera->ProcessMouseMovement(xoffset, yoffset, true);
    }
}

void onError(int error, const char *description) {
    std::cerr << "[APPLICATION] Error:" << error << "description: " << description << std::endl;
}

void showFPS(EinWindow* pWindow) {
    // Measure speed
    static double lastTime;
    static int nbFrames;
    double currentTime = glfwGetTime();
    double delta = currentTime - lastTime;
    nbFrames++;
    if ( delta >= 1.0 ) { // If last cout was more than 1 sec ago
        // std::cout << 1000.0/double(nbFrames) << std::endl;

        double fps = double(nbFrames) / delta;

        std::stringstream ss;
        ss << "cgBentRendering [" << fps << " FPS]";

        pWindow->SetTitle(ss.str());

        nbFrames = 0;
        lastTime = currentTime;
    }
}
