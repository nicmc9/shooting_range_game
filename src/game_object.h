#pragma once

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
  
    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, float radius = 0.0f,
               glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity  =  glm::vec2(0.0f, 0.0f), float health = 1.0f );
    virtual ~GameObject() = default;

    
    //содержит функцию отрисовки т.е. управляет своим рисованием
    virtual void Draw(SpriteRenderer &renderer);
    virtual void Draw(SpriteRenderer &renderer, glm::vec2 origin);
    virtual void Move(float dt, unsigned int window_width,unsigned int window_height);
   
    void       set_position_bound(glm::vec2 pos, unsigned int width, unsigned int height);

public:
  // Состояния рендер
    Texture2D   sprite_;	
    glm::vec2   position_, size_; 
    glm::vec3   color_;
    float       rotation_;
    //состояния игровой логики
    float       radius_;
    glm::vec2   velocity_;
    bool        activated_;
    bool        destroyed_;
    float       health_;
};