//----------------------------------------------------------------------------------------
/**
 * \file    Main.cpp
 * \author  Veronika Shirochenkova
 * \date    2022
 * \brief   Implementaion of the scene "The Witch's room".
 */
 //----------------------------------------------------------------------------------------

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "Parameters.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Bone.h"
#include "Cube.h"
#include "Light.h"
#include "Skybox.h"
#include "Flame.h"
#include "Potion.h"
#include "Glass.h"
#include "Clock.h"
#include "Crosshair.h"

#include <iostream>


bool CURSOR_DISABLED = true;        // cursor status 
bool LIGHT_ON = true;               // turn on/off point light
bool CHANGE_COLOR_POTION = false;   // interact with main kettle
bool BALL_ANIMATION = false;        // turn on/off magic ball animation
bool FLASHLIGHT = false;            // turn on/off flashlight
bool FOG = false;                   // turn on/off fog
bool COLLISION = false;             // turn on/off collision
bool HUD = false;                   // turn on/off crosshair/timer

// timing
float deltaTime = 0.0f;
float prevFrame = 0.0f;

Camera camera(staticViewPos[0]);
// last camera screen position
float lastX = WIN_WIDTH / 2.0f;
float lastY = WIN_HEIGHT / 2.0f;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void process_input(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int key, int action, int mods);
void startInfo();
void exitInfo();


int main()
{
    // INITIALIZATION
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "The Witch's Room", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // PRINT START INFO TO CONSOLE
    startInfo();

    // SHADERS
    Shader modelShader("Models.vert", "Models.frag");
    Shader skyboxShader("Skybox.vert", "Skybox.frag");
    Shader boneShader("Bone.vert", "Bone.frag");
    Shader flameShader("Flame.vert", "Flame.frag");
    Shader potionShader("Potion.vert", "Potion.frag");
    Shader cubeShader("Glass.vert", "Glass.frag");
    Shader windowShader("Glass.vert", "Glass.frag");
    Shader clockShader("Clock.vert", "Clock.frag");


    // SKYBOX
    Skybox skybox;
    skybox.loadTexture(skyboxFaces);
    skybox.create();

    // CAMERA
    camera.setupStartView();

    // MODELS
    std::vector <Model> Models;
    loadModels(&Models);

    // FLAME
    Flame flame(FLAME_NUM_OF_FRAMES);
    flame.loadTexture();
    flame.create();

    // BONE
    Bone bone(BONE_POSITION, BONE_SPEED);
    bone.create();

    // POTION
    Potion potion(POTION_POSITION);
    potion.loadTexture();
    potion.create();

    // FLYING CUBE
    Cube cube(CUBE_SPEED);
    cube.create();

    // GLASS
    Glass glass;
    glass.create();

    // CLOCK
    Clock clock;
    clock.create();
    clock.loadTexture();

    // CROSSHAIR
    Crosshair crosshair;
    crosshair.create();
    crosshair.loadTexture();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // MAIN RENDER LOOP
    while (!glfwWindowShouldClose(window))
    {
        // TIME
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - prevFrame;
        prevFrame = currentFrame;

        // INPUT
        process_input(window);

        // RENDER
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


        // SKYBOX
        glDepthMask(GL_FALSE);
        skyboxShader.use();
        glm::mat4 viewS = glm::mat4(glm::mat3(glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up)));
        glm::mat4 projectionS = glm::perspective(glm::radians(camera.Zoom), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(viewS));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projectionS));
        glBindVertexArray(skybox.VAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.texture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);

        // MAIN SHADER
        modelShader.use();
        setLightUniforms(&modelShader, &camera);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        glm::mat4 view = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);

        glUniformMatrix4fv(glGetUniformLocation(modelShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(modelShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(modelShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glUniform3fv(glGetUniformLocation(modelShader.ID, "viewPos"), 1, glm::value_ptr(camera.Position));
        glUniform1i(glGetUniformLocation(modelShader.ID, "light_on"), LIGHT_ON);
        glUniform1i(glGetUniformLocation(modelShader.ID, "flashlight"), FLASHLIGHT);
        glUniform1i(glGetUniformLocation(modelShader.ID, "fog"), FOG);
        glUniform1f(glGetUniformLocation(modelShader.ID, "global_time"), fmodf(currentFrame, 24));

        glUniform1i(glGetUniformLocation(modelShader.ID, "scale"), false);
        glUniform1f(glGetUniformLocation(modelShader.ID, "time"), fmodf(currentFrame, 6));


        // MODELS
        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        for (int i = 0; i < Models.size(); i++) {
            glStencilFunc(GL_ALWAYS, i + 1, -1);
            if (i == 18) {
                if (BALL_ANIMATION) {
                    glUniform1i(glGetUniformLocation(modelShader.ID, "scale"), true);
                }
                else {
                    glUniform1i(glGetUniformLocation(modelShader.ID, "scale"), false);
                }
                
                glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.18f, 0.88f, -0.84f));
                glUniformMatrix4fv(glGetUniformLocation(modelShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
            }
            else {
                glUniform1i(glGetUniformLocation(modelShader.ID, "scale"), false);
                glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
                glUniformMatrix4fv(glGetUniformLocation(modelShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
            }
            Models[i].draw(modelShader);
        }
        glDisable(GL_STENCIL_TEST);


        // BONE
        boneShader.use();
        setLightUniforms(&boneShader, &camera);
        bone.animation();

        model = glm::translate(glm::mat4(1.0f), glm::vec3(bone.xPos, bone.yPos, bone.zPos));
        model = glm::rotate(model, glm::radians(100.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(bone.selfRotation), glm::vec3(0.0f, 0.0f, 1.0f));

        glUniformMatrix4fv(glGetUniformLocation(boneShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(boneShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(boneShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glUniform3fv(glGetUniformLocation(boneShader.ID, "viewPos"), 1, glm::value_ptr(camera.Position));
        glUniform1i(glGetUniformLocation(boneShader.ID, "fog"), FOG);
        glUniform1i(glGetUniformLocation(boneShader.ID, "light_on"), LIGHT_ON);
        glUniform1i(glGetUniformLocation(boneShader.ID, "flashlight"), FLASHLIGHT);
        glUniform1f(glGetUniformLocation(boneShader.ID, "global_time"), fmodf(currentFrame, 24));

      
        glBindVertexArray(bone.VAO);    
        glDrawElements(GL_TRIANGLES, 3 * 96, GL_UNSIGNED_INT, 0);


        // FLAME
        if (LIGHT_ON) 
        {
            flameShader.use();
            flame.animation(FLAME_DELAY);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, flame.texture);
            glUniform1i(glGetUniformLocation(flameShader.ID, "texture1"), 0);

            glUniform1i(glGetUniformLocation(flameShader.ID, "frame"), flame.currentFrame);

            glUniformMatrix4fv(glGetUniformLocation(flameShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(flameShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
            
            glBindVertexArray(flame.VAO);

            for (unsigned int i = 0; i < POINT_LIGHT_POS; i++) 
            {
                model = glm::translate(glm::mat4(1.0f), pointLightPositions[i]);

                model = glm::scale(model, glm::vec3(0.1f));
                glUniformMatrix4fv(glGetUniformLocation(flameShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                glUniformMatrix4fv(glGetUniformLocation(flameShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }

            
        }


        // POTION
        potionShader.use();
        if (CHANGE_COLOR_POTION) {
            potion.changeColor();
            CHANGE_COLOR_POTION = false;
            std::cout << "== Potion color changed! Now it's " << potionColors[potion.color] << "!" << std::endl;
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, potion.texture);
        glUniform1i(glGetUniformLocation(potionShader.ID, "texture1"), 0);

        model = glm::translate(glm::mat4(1.0f), potion.position);
        model = glm::scale(model, glm::vec3(0.332f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        glUniformMatrix4fv(glGetUniformLocation(potionShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(potionShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(potionShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


        glUniform3fv(glGetUniformLocation(potionShader.ID, "viewPos"), 1, glm::value_ptr(camera.Position));
        glUniform1i(glGetUniformLocation(potionShader.ID, "fog"), FOG);
        glUniform1f(glGetUniformLocation(potionShader.ID, "global_time"), fmodf(currentFrame, 24));     
        glUniform4fv(glGetUniformLocation(potionShader.ID, "potion_color"), 1, glm::value_ptr(potionColor[potion.color]));
        glUniform1f(glGetUniformLocation(potionShader.ID, "time"), currentFrame);

        glBindVertexArray(potion.VAO);
        glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);


        // FLYING CUBE
        cubeShader.use();

        cube.ñurveAnimation();
        cube.time();

        model = glm::translate(glm::mat4(1.0f), cube.position);
        model = glm::scale(model, glm::vec3(0.02f));
        model = glm::rotate(model, glm::radians(cube.rotation), glm::vec3(0.0f, 1.0f, 1.0f));

        glUniformMatrix4fv(glGetUniformLocation(cubeShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(cubeShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(cubeShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(cube.VAO); 
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // WINDOW
        windowShader.use();

        model = glm::translate(glm::mat4(1.0f), WINDOW_POSITION);

        glUniformMatrix4fv(glGetUniformLocation(windowShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(windowShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(windowShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(glass.VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        
        if (HUD) {
            glDepthMask(GL_FALSE);
            // CLOCK
            clockShader.use();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, clock.texture);
            glUniform1i(glGetUniformLocation(clockShader.ID, "texture1"), 0);

            if ((int)glfwGetTime() > clock.animationTime) {
                clock.rotation += 6.0f;
                clock.animationTime = (int)glfwGetTime();
            }

            model = glm::translate(glm::mat4(1.0f), glm::vec3(0.92f, -0.87f, 0.0f));
            model = glm::scale(model, glm::vec3(0.065f, 0.1f, 0.0f));
            model = glm::rotate(model, glm::radians(-clock.rotation), glm::vec3(0.0f, 0.0f, 1.0f));
            glUniformMatrix4fv(glGetUniformLocation(clockShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

            glBindVertexArray(clock.VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


            // CROSSHAIR
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, crosshair.texture);
            glUniform1i(glGetUniformLocation(clockShader.ID, "texture1"), 0);

            model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.2f, 0.3f, 0.0f));
            glUniformMatrix4fv(glGetUniformLocation(clockShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

            glBindVertexArray(crosshair.VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glDepthMask(GL_TRUE);
        }


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // PRINT EXIT INFO TO CONSOLE
    exitInfo();

    glfwTerminate();
    return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // CLOSE WINDOW
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // STATIC -> DYNAMIC VIEW
    if (key == GLFW_KEY_G && action == GLFW_PRESS && camera.staticView) {
        camera.staticView = false;
        camera.StaticView2 = false;
        camera.StaticView1 = false;
        std::cout << "== Dynamic view;" << std::endl;
    }

    // STATIC VIEW 1
    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        if (!camera.staticView) {
            camera.staticView = true;
            camera.firstMouseUse = true;
        }
        if (!camera.StaticView1) {
            camera.StaticView1 = true;
            camera.StaticView2 = false;
            std::cout << "== First static view;" << std::endl;
        }
        camera.Position = staticViewPos[0];
        camera.Yaw = staticViewAngles[0][0];
        camera.Pitch = staticViewAngles[0][1];
        camera.updateCameraVectors();

    }
    // STATIC VIEW 2
    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        if (!camera.staticView) {
            camera.staticView = true;
            camera.firstMouseUse = true;
        }
        if (!camera.StaticView2) {
            camera.StaticView2 = true;
            camera.StaticView1 = false;
            std::cout << "== Second static view;" << std::endl;
        }
        camera.Position = staticViewPos[1];
        camera.Yaw = staticViewAngles[1][0];
        camera.Pitch = staticViewAngles[1][1];
        camera.updateCameraVectors();
    }

    // CURSOR
    if (key == GLFW_KEY_C && action == GLFW_PRESS && CURSOR_DISABLED) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        CURSOR_DISABLED = false;
        std::cout << "== Now your cursor is free!" << std::endl;
    }
    else if (key == GLFW_KEY_C && action == GLFW_PRESS && !CURSOR_DISABLED) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        CURSOR_DISABLED = true;
        std::cout << "== Now your cursor works in the window again!" << std::endl;
    }

    // FLASHLIGHT
    if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        if (FLASHLIGHT) {
            FLASHLIGHT = false;
            std::cout << "== Now you've stopped using the flashlight;" << std::endl;
        }
        else {
            FLASHLIGHT = true;
            std::cout << "== Now you use a flashlight;" << std::endl;
        }
    }

    // FOG
    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        if (FOG) {
            FOG = false;
            std::cout << "== The fog is OFF;" << std::endl;
        }
        else {
            FOG = true;
            std::cout << "== The fog is ON;" << std::endl;
        }
    }

    // COLLISION
    if (key == GLFW_KEY_K && action == GLFW_PRESS) {
        if (COLLISION) {
            COLLISION = false;
            std::cout << "== The collision is OFF;" << std::endl;
        }
        else {
            COLLISION = true;
            std::cout << "== The collision is ON;" << std::endl;
        }
    }

    // HUD
    if (key == GLFW_KEY_H && action == GLFW_PRESS) {
        if (HUD) {
            HUD = false;
            std::cout << "== You turned off the HUD!" << std::endl;
        }
        else {
            HUD = true;
            std::cout << "== You turned on the HUD!" << std::endl;
        }
    }

}



void mouse_button_callback(GLFWwindow* window, int key, int action, int mods)
{
    // PICK
    if (!camera.staticView) {
        unsigned char objectID = 0;
        unsigned int depth = 0;

        glReadPixels(WIN_WIDTH / 2, WIN_HEIGHT / 2, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &objectID);
        glReadPixels(WIN_WIDTH / 2, WIN_HEIGHT / 2, 1, 1, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, &depth);

        // CANDLES
        if ((int)objectID == 12 && key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            if (depth < 220) {
                if (LIGHT_ON) {
                    LIGHT_ON = false;
                    std::cout << "== You put out the candles!" << std::endl;
                }
                else {
                    LIGHT_ON = true;
                    std::cout << "== You lit the candles!" << std::endl;
                }
            }
            
        }

        // MAGIC BALL
        if ((int)objectID == 19 && key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            if (depth < 220) {
                std::cout << "== You touch the magic ball!" << std::endl;
                if (BALL_ANIMATION) {
                    BALL_ANIMATION = false;
                }
                else {
                    BALL_ANIMATION = true;
                }
                
            }
            
        }

        // POTION COLOR
        if ((int)objectID == 22 && key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            if (depth < 220) {
                CHANGE_COLOR_POTION = true;
            }    
        }
    }
}

void process_input(GLFWwindow* window)
{
    // CAMERA
    if (!camera.staticView) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.processKeyboard(FORWARD, deltaTime, COLLISION);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.processKeyboard(BACKWARD, deltaTime, COLLISION);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.processKeyboard(LEFT, deltaTime, COLLISION);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.processKeyboard(RIGHT, deltaTime, COLLISION);
    }

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (!camera.staticView) {

        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (camera.firstMouseUse)
        {
            lastX = xpos;
            lastY = ypos;
            camera.firstMouseUse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;

        lastX = xpos;
        lastY = ypos;

        camera.processMouseMovement(xoffset, yoffset);
    }

}

void startInfo()
{

    std::cout << "   _____ _            _    _ _ _       _     _      ______                        " << std::endl;
    std::cout << "  |_   _| |          | |  | (_) |     | |   ( )     | ___ \\                       " << std::endl;
    std::cout << "    | | | |__   ___  | |  | |_| |_ ___| |__ |/ ___  | |_/ /___   ___  _ __ ___    " << std::endl;
    std::cout << "    | | | '_ \\ / _ \\ | |/\\| | | __/ __| '_ \\  / __| |    // _ \\ / _ \\| '_ ` _ \\   " << std::endl;
    std::cout << "    | | | | | |  __/ \\  /\\  / | || (__| | | | \\__ \\ | |\\ \\ (_) | (_) | | | | | |  " << std::endl;
    std::cout << "    \\_/ |_| |_|\\___|  \\/  \\/|_|\\__\\___|_| |_| |___/ \\_| \\_\\___/ \\___/|_| |_| |_|  " << std::endl;
    std::cout << "\n" << std::endl;


}

void exitInfo()
{
    std::cout << "\n" << std::endl;
    std::cout << "   ______             ______             " << std::endl;
    std::cout << "   | ___ \\            | ___ \\            " << std::endl;
    std::cout << "   | |_/ /_   _  ___  | |_/ /_   _  ___  " << std::endl;
    std::cout << "   | ___ \\ | | |/ _ \\ | ___ \\ | | |/ _ \\ " << std::endl;
    std::cout << "   | |_/ / |_| |  __/ | |_/ / |_| |  __/" << std::endl;
    std::cout << "   \\____ /\\__, |\\___| \\____/ \\__, |\\___|" << std::endl;
    std::cout << "           __/ |              __/ |      " << std::endl;
    std::cout << "          |___/              |___/      " << std::endl;
    std::cout << "\n" << std::endl;
}
