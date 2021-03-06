﻿/*******************************************************************
** This code is part of ShootingRange game.
** Created Baleyv Nikolay nicmc9@gmail.com
**
** ShootingRange is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "sprite_renderer.h"

SpriteRenderer::SpriteRenderer(Shader &shader)
{
    shader_ = shader;
    InitRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
   // glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::DrawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color, glm::vec2 origin)
{
   
    //активация шейдера
    shader_.Use();
    glm::mat4 model = glm::mat4(1.0f);

    /*
    Не забываем про порядок трансформаций сначала. перемещение,затем поворот, затем масшаб
    т.к.  применяться они будут в обратном порядке
    */
    model = glm::translate(model, glm::vec3(position, 0.0f));     
    model = glm::translate(model, glm::vec3(origin.x * size.x, origin.y * size.y, 0.0f)); // возвращаем обратно
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); //поворачиваем 
    model = glm::translate(model, glm::vec3(-origin.x * size.x, -origin.y * size.y, 0.0f)); //переность ось вращения к ценру квадрата 

    model = glm::scale(model, glm::vec3(size, 1.0f)); // масштабируем

    /*юниформов  модели цвета и самплера текстуры , все кроме проекции которая однакова для всех спрайто
      поэтому в инит game класс  
    */
    shader_.SetMatrix4("model", model);
    shader_.SetVector3f("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(quad_vao_);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}


void SpriteRenderer::InitRenderData()
{
    // configure VAO/VBO
    GLuint vbo;
    float vertices[] = { 
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &quad_vao_);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(quad_vao_);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}