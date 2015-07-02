#include <iostream>
#include <time.h>
#include <string>
#include <sstream>

#include "utility/Ein.hpp"

// Other includes
#include "utility/Camera.hpp"
#include "utility/Shader.hpp"
#include "utility/Texture.hpp"
#include "utility/Model.hpp"
#include "utility/Framebuffer.hpp"

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
GLint nbFrames = 0;

struct Params {
    float sampleRadius;
    float maxDistance;
    int patternSize;
    int sampleCount;
    int numRayMarchingSteps;
    float rayMarchingBias;
};

void onError(int error, const char *description);
void showFPS(EinWindow* pWindow);
void doMovement(EinInputManager* inputManager, Camera *camera);
void unitSphericalToCarthesian(const glm::vec2& spherical, glm::vec3& result);
void inline srandTimeSeed() {
    srand(time(NULL));
}
float inline randMToN(float M, float N)
{
    return M + (float(rand()) / ( float(RAND_MAX) / (N-M) ) ) ;
}
float inline frand() {
    return (float(rand()) / float(RAND_MAX));
}
float inline lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

/*
 * BASIC IDEA:
 * Create framebuffer object, bind a depth attachment to it and set glDraw to none so is ready
 * Use a shader that in the vertex shader passes the position in screen coordinate (MVP*position)
 * and in the fragment shader outputs the gl_FragCoodinate.z value (the depth value)
 * Bind framebuffer, use shader, draw scene, unbind framebuffer
 * Create framebuffer object, bind a color attachment, type RGBA
 * Use a shader that calculates the bent normals for each fragment, storing the bent normal in rgb
 * and the ambient occlusion value in a
 * Bind framebuffer, use shader, draw scene, unbind framebuffer
 * Use a shader that does a basic lighting tecnique, like Blinn-Phong, but uses the normals stored in
 * the color attachment texture of the previous passage
 *
 * 1- Create a texture containing the deph values of the scene
 * 2- Use this texture as imput for the shader calculating the bent normals,
 *    saved in another texture just like a normal map
 * 3- Use the "bent normal map" to render the scene using the classic
 *    Blinn-Phong tecnique
 */

int main()
{
    EinApplication::setErrorCallback(onError);
    EinApplication *app = new EinApplication();

    EinWindow* window = new EinWindow(WIN_WIDTH, WIN_HEIGHT, "cgBentRendering", false);

    Camera* mainCamera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

    GLfloat quadVertices[] = {   // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // Positions   // TexCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    // Setup screenQuad VAO
    GLuint quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glBindVertexArray(0);

    Shader modelGbuffer("shaders/modelGbuffer.vert", "shaders/modelGbuffer.frag");
    modelGbuffer.Link();

    // Shader bentNormalsShader("shaders/quad.vert", "shaders/bent_normals.frag");
    // bentNormalsShader.Link();
    // // algorithm params
    // Params params;
    // params.sampleRadius = 1.0f;
    // params.maxDistance = params.sampleRadius * 1.6f;
    // params.numRayMarchingSteps = 3;
    // params.patternSize = 8;
    // params.sampleCount = 8;
    // params.rayMarchingBias = params.sampleRadius / float(params.numRayMarchingSteps) / 1000.0f;
    //
    // glUniform1f(glGetUniformLocation(bentNormalsShader.Program, "sampleRadius"), params.sampleRadius);
    // glUniform1f(glGetUniformLocation(bentNormalsShader.Program, "maxDistance"), params.maxDistance);
    // glUniform1i(glGetUniformLocation(bentNormalsShader.Program, "patternSize"), params.patternSize);
    // glUniform1i(glGetUniformLocation(bentNormalsShader.Program, "sampleCount"), params.sampleCount);
    // glUniform1i(glGetUniformLocation(bentNormalsShader.Program, "numRayMarchingSteps"), params.numRayMarchingSteps);
    //
    // int kernelSize = 32;
    // glm::vec3 *kernel = new glm::vec3[kernelSize];
    // srandTimeSeed();
    // for (int i = 0; i < kernelSize; i++) {
    //     kernel[i] = glm::vec3(
    //             randMToN(-1.0f, 1.0f),
    //             randMToN(-1.0f, 1.0f),
    //             randMToN(0.0f, 1.0f));
    //     kernel[i] = glm::normalize(kernel[i]);
    //
    //     float scale = (float)i / (float)kernelSize;
    //     kernel[i] *= lerp(0.1f, 1.0f, scale * scale);
    // }
    //
    // for (int i = 0; i< kernelSize; i++) {
    //     std::cerr << kernel[i].x <<  " " << kernel[i].y << " " << kernel[i].z << std::endl;
    // }
    // glUniform1i(glGetUniformLocation(bentNormalsShader.Program, "uKernelSize"), kernelSize);
    // glUniform3fv(glGetUniformLocation(bentNormalsShader.Program, "uKernelOffsets"), kernelSize, (const GLfloat*)glm::value_ptr(kernel[0]));
    //
    // int ssaoNoiseSize = 8;
    // int noiseDataSize = ssaoNoiseSize * ssaoNoiseSize;
    // glm::vec3 *noiseData = new glm::vec3[noiseDataSize];
    // srandTimeSeed();
    // for (int i = 0; i < noiseDataSize; i++) {
    //     noiseData[i] = glm::vec3(
    //             randMToN(-1.0f, 1.0f),
    //             randMToN(-1.0f, 1.0f),
    //             0.0f);
    //     noiseData[i] = glm::normalize(noiseData[i]);
    // }
    // Texture noiseTex;
    //
    // noiseTex.Bind();
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ssaoNoiseSize, ssaoNoiseSize, 0, GL_RGB, GL_FLOAT, noiseData);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // noiseTex.Unbind();

    Shader lightingShader("shaders/quad.vert", "shaders/lighting.frag");
    lightingShader.Link();

    Shader simpleShader("shaders/simple.vert", "shaders/simple.frag");
    simpleShader.Link();

    // Load models
    // Model demonHeadModel("models/nanosuit/nanosuit.obj");
    Model demonHeadModel("models/bake/monkey2.obj");

    Texture bentNormalsTexture("models/bake/thebake.png", GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);

    // Setup gBuffer
    Framebuffer *gBuffer = new Framebuffer();
    gBuffer->BindFb();
    gBuffer->addTextureAttachment(TexAttachmentType::DEPTH, window->GetFramebufferSize());
    gBuffer->addTextureAttachment(TexAttachmentType::RGBA, window->GetFramebufferSize(), "position");
    gBuffer->addTextureAttachment(TexAttachmentType::RGB, window->GetFramebufferSize(), "normal");
    gBuffer->addTextureAttachment(TexAttachmentType::RGB, window->GetFramebufferSize(), "color");
    gBuffer->setupMRT();
    if(!gBuffer->isReady())
        std::cerr << "Gbuffer incomplete" << std::endl;
    gBuffer->UnbindFb();
    // Get textures from gBuffer
    Texture depthTex = gBuffer->getTextureAttachment(DEPTH);
    Texture positionTex = gBuffer->getTextureAttachment("position");
    Texture normalTex = gBuffer->getTextureAttachment("normal");
    Texture colorTex = gBuffer->getTextureAttachment("color");

    // Setup bent_normalsBuffer
    Framebuffer *bentNormalsBuffer = new Framebuffer();
    bentNormalsBuffer->BindFb();
    bentNormalsBuffer->addTextureAttachment(TexAttachmentType::RGBA, window->GetFramebufferSize(), "bent_normals");
    if(!bentNormalsBuffer->isReady())
        std::cerr << "Bent normals buffer incomplete" << std::endl;
    bentNormalsBuffer->UnbindFb();
    // Get bent normals texture
    Texture bentNormalsTex = bentNormalsBuffer->getTextureAttachment("bent_normals");

    bool useNormalMap = false;

    while(!window->ShouldClose())
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // Pool events
        window->GetInputManager()->pollEvents();
        if((window->GetInputManager())->isExit(KeyActionType::KEY_UP)) {
            window->SetShouldClose(true);
        }
        if((window->GetInputManager())->isKey(GLFW_KEY_T, KeyActionType::KEY_UP)) {
            if (useNormalMap)
                useNormalMap = false;
            else
                useNormalMap = true;
        }
        // Handle camera movements
        doMovement(window->GetInputManager(), mainCamera);

        // Bind gBuffer
        gBuffer->BindFb();
        // Clear the screen to black
        // Setup OpenGL options
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //  Activate shader
        modelGbuffer.Use();

        // Create transformations
        glm::mat4 view = mainCamera->GetViewMatrix();
        glm::mat4 projection = glm::perspective(mainCamera->zoomQuantity, (float)(window->GetWindowSize().width)/(float)(window->GetWindowSize().height), 0.1f, 10.0f);
        glm::mat4 model;
        // model = glm::translate(model, glm::vec3(0.0f, -2.7f, 2.3f));
        // model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(modelGbuffer.Program, "model");
        GLint viewLoc = glGetUniformLocation(modelGbuffer.Program, "view");
        GLint projLoc = glGetUniformLocation(modelGbuffer.Program, "projection");
        // Pass the matrices to the shader
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        
        glActiveTexture(GL_TEXTURE0);
        bentNormalsTexture.Bind();
        glUniform1i(glGetUniformLocation(modelGbuffer.Program, "normalMap"), 0);
        glUniform1i(glGetUniformLocation(modelGbuffer.Program, "useNormalMap"), useNormalMap);
        demonHeadModel.Draw(modelGbuffer);

        gBuffer->UnbindFb();

        // bentNormalsBuffer->BindFb();
        // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT);
        // glDisable(GL_DEPTH_TEST);
        //
        // bentNormalsShader.Use();
        // glBindVertexArray(quadVAO);
        // GLint viewMatrixLoc = glGetUniformLocation(bentNormalsShader.Program, "viewMatrix");
        // GLint viewProjectionMatrixLoc = glGetUniformLocation(bentNormalsShader.Program, "projectionMatrix");
        // // Pass the matrices to the shader
        // glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(view));
        // glUniformMatrix4fv(viewProjectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(projection));
        //
        // glActiveTexture(GL_TEXTURE0);
        // positionTex.Bind();
        // glUniform1i(glGetUniformLocation(bentNormalsShader.Program, "positionTexture"), 0);
        //
        // glActiveTexture(GL_TEXTURE1);
        // normalTex.Bind();
        // glUniform1i(glGetUniformLocation(bentNormalsShader.Program, "normalTexture"), 1);
        //
        // glActiveTexture(GL_TEXTURE2);
        // depthTex.Bind();
        // glUniform1i(glGetUniformLocation(bentNormalsShader.Program, "depthTexture"), 2);
        //
        // glActiveTexture(GL_TEXTURE3);
        // noiseTex.Bind();
        // glUniform1i(glGetUniformLocation(bentNormalsShader.Program, "uNoiseTex"), 3);
        // glDrawArrays(GL_TRIANGLES, 0, 6);
        // glBindVertexArray(0);
        // bentNormalsBuffer->UnbindFb();

        // Activate shader

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        // simpleShader.Use();
        // GLint projMatLoc = glGetUniformLocation(simpleShader.Program, "projection");
        // std::cerr << "valori matrice projection " << projection[2][2] << " e " << projection[3][2] << std::endl;
        // glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, glm::value_ptr(projection));
        lightingShader.Use();
        glBindVertexArray(quadVAO);
        glActiveTexture(GL_TEXTURE0);
        // depthTex.Bind();
        // bentNormalsTex.Bind();
        // glUniform1i(glGetUniformLocation(bentNormalsShader.Program, "screenTexture"), 0);
        positionTex.Bind();
        glUniform1i(glGetUniformLocation(lightingShader.Program, "tPosition"), 0);
        glActiveTexture(GL_TEXTURE1);
        normalTex.Bind();
        glUniform1i(glGetUniformLocation(lightingShader.Program, "tNormals"), 1);
        glActiveTexture(GL_TEXTURE2);
        colorTex.Bind();
        glUniform1i(glGetUniformLocation(lightingShader.Program, "tDiffuse"), 2);
        glActiveTexture(GL_TEXTURE3);
        bentNormalsTex.Bind();
        glUniform1i(glGetUniformLocation(lightingShader.Program, "tBentNormals"), 3);
        GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "lightPos");
        GLint viewerPosLoc = glGetUniformLocation(lightingShader.Program, "viewerPos");
        glUniform3f(viewerPosLoc, mainCamera->position.x, mainCamera->position.y, mainCamera->position.z);
        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // Swap screen buffer
        window->SwapBuffers();
        showFPS(window);
    }

    // Clean up resources
    lightingShader.Delete();
    modelGbuffer.Delete();

    glDeleteVertexArrays(1, &quadVAO);
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
    static int frames = 0;
    static double starttime = 0;
    static bool first = true;
    static float fps = 0.0f;

    if (first)
    {
        frames = 0;
        starttime = glfwGetTime();
        first = false;
        return;
    }
    frames++;
    double tempTime = glfwGetTime();
    if (tempTime - starttime > 0.25 && frames > 10)
    {
        fps = (double) frames / (tempTime - starttime);
        starttime = tempTime;
        std::stringstream ss;
        ss << "cgBentRendering [" << fps << " FPS]";

        pWindow->SetTitle(ss.str());
        frames = 0;
    }
}

void unitSphericalToCarthesian(const glm::vec2& spherical, glm::vec3& result) {
    const float phi = spherical.x;
    const float theta = spherical.y;
    result.x = sinf(phi) * sinf(theta);
    result.y = cosf(phi) * sinf(theta);
    result.z = cosf(theta);
}
