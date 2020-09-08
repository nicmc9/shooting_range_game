/*******************************************************************
** This code is part of ShootingRange game.
** Created Baleyv Nikolay nicmc9@gmail.com
**
** ShootingRange is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"
#include "utility.h"
/*
Класс отвечающий за отрисовку спрайтов
Создает VAO по умолчанию . а всю информацию для отрисовки
мы передаем в DrawSprite тем самы используя тот же VAO и шейдер
но получая разные по виду и положению спрайты
*/
class SpriteRenderer
{
public:
     
    SpriteRenderer(Shader &shader);
    ~SpriteRenderer();
    /* передаем текстуру которая накладывается на данных спрайт, 
       позиция размер и угол поворота нужны для матрицы модели
       цвет передается в шейдер т.к текстуры мы загружаем в градациях серого 
    */
    void DrawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f),
                    float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f), glm::vec2 origin = glm::vec2(0.5f,0.5f));
private:

    Shader       shader_;  
    GLuint quad_vao_;
    //Настраиваем буферы данных для спрайта
    void InitRenderData();
};

