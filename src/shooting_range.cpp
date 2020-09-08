/*******************************************************************
** This code is part of ShootingRange game.
** Created Baleyv Nikolay nicmc9@gmail.com
**
** ShootingRange is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <iostream>

#include "resource_manager.h"
#include "game.h"

// GLFW function declerations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);


const unsigned int kScreenWidth = 1024;
const unsigned int kScreenHeight = 768;


Game shooting_range(kScreenWidth, kScreenHeight);

int main()
{
    setlocale(LC_ALL, "Russian");
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow* window = glfwCreateWindow(kScreenWidth, kScreenHeight, "Shooting Range", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetCursorPos(window, kScreenWidth / 2.0f, kScreenHeight / 2.0f);
    glfwSetMouseButtonCallback(window, mouse_button_callback);


    // OpenGL configuration
    // --------------------
    glViewport(0, 0, kScreenWidth, kScreenHeight);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  //Инициализация игры и состояния
    shooting_range.Init();
    shooting_range.set_state(GameState::kGameMenu);
    
    // deltaTime variables
    // -------------------
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();
        
        shooting_range.ProcessInput(deltaTime);
        shooting_range.Update(deltaTime);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
     
        shooting_range.Render();
        glfwSwapBuffers(window);

    }

 // Очиска всех загруженных ресурсов
    ResourceManager::Clear();

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
   
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            shooting_range.set_keys(key, true);
        else if (action == GLFW_RELEASE){
            shooting_range.set_keys(key, false);
            shooting_range.set_keys_processed(key,false);
        }
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
   glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	//print("xpos ",xpos,"ypos", ypos);
    shooting_range.MouseInput(xpos, ypos);
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){

    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        shooting_range.MouseButtonClick();
}

