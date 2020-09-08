/*******************************************************************
** This code is part of ShootingRange game.
** Created Baleyv Nikolay nicmc9@gmail.com
**
** ShootingRange is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/

#include "game_object.h"

GameObject::GameObject() 
    : position_(0.0f, 0.0f), size_(1.0f, 1.0f), velocity_(0.0f),
      color_(1.0f), rotation_(0.0f), radius_(0.0f), sprite_(),
      health_(1.0f), destroyed_(false), activated_(true) { }

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite,
      float radius, glm::vec3 color, glm::vec2 velocity, float health) 
    : position_(pos), size_(size), velocity_(velocity), color_(color),
      rotation_(0.0f), sprite_(sprite),radius_(radius), health_(health),
      destroyed_(false), activated_(true) { }

void GameObject::Draw(SpriteRenderer &renderer)
{
    renderer.DrawSprite(sprite_, position_, size_, rotation_, color_);
}

void GameObject::Draw(SpriteRenderer &renderer, glm::vec2 origin)
{
    renderer.DrawSprite(sprite_, position_, size_, rotation_, color_, origin);
}


void GameObject::Move(float dt, unsigned int window_width, unsigned int window_height)
{
        //меняем вид движения у 3 й цели , возможно сделать через наследование, но так гораздо меньше кода
        if(velocity_.x == 200.0f){
          position_.x += velocity_.x*dt; 
          position_.y += glm::cos((float)glfwGetTime()) * 2;   
        }else{
           position_ += velocity_*dt; 
        }

        //Пооверка достижения границ экрана
        if(position_.x <= 0.0f)
        {
            velocity_.x = - velocity_.x; // меняем направление скорости
            position_.x = 0.0f;
        }
        else if (position_.x + size_.x >= window_width)
        {
            velocity_.x = -velocity_.x;
            position_.x = window_width - size_.x;
        }
        if(position_.y <= 0.0f)
        {
            velocity_.y = - velocity_.y;
            position_.y = 0.0f;
        }
        else if(position_.y + size_.y >= window_height-size_.y){
            velocity_.y = - velocity_.y;
            position_.y = window_height - size_.y*2;
        }
}

void    GameObject::set_position_bound(glm::vec2 pos, unsigned int width, unsigned int height ){
        position_  = pos; 

    //ПРоверка достижения границ экрана
        if(position_.x <= 0.0f)  
            position_.x = 0.0f;
        else if (position_.x + size_.x >= width)
            position_.x =  width - size_.x;
        if( position_.y <= 0.0f)
            position_.y = 0.0f;
        else if(position_.y + size_.y >= height)
            position_.y = height - size_.y;
}