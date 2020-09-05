﻿#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>


#include "texture.h"
#include "sprite_renderer.h"

/*
 Контейнер игрового объекта (Актор), содержит все неоходимое для
 отрисовки и управления каждой игровой сущности в игре,
  может быть базовым классом и храниться в объекте уровня
*/

class GameObject
{
public:
    // Состояния рендер
    Texture2D   Sprite;	
    glm::vec2   Position, Size; 
    glm::vec3   Color;
    float       Rotation;
    float       Radius;
    //состояния игровой логики
    glm::vec2   Velocity;
    bool        Spawned;
    bool        Destroyed;
   
   
    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, float radius = 0.0f,
               glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity  =  glm::vec2(0.0f, 0.0f) );
    virtual ~GameObject() = default;

    
    //содержит функцию отрисовки т.е. управляет своим рисованием
    virtual void Draw(SpriteRenderer &renderer);
    virtual void DrawOrigin(SpriteRenderer &renderer);
    virtual glm::vec2 Move(float dt, unsigned int window_width,unsigned int window_height);
};