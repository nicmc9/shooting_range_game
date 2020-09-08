/*******************************************************************
** This code is part of ShootingRange game.
** Created Baleyv Nikolay nicmc9@gmail.com
**
** ShootingRange is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/

#include "bullet_object.h"

BulletObject::BulletObject(): GameObject(), start_position_(0.0f) { }

BulletObject::BulletObject(glm::vec2 pos,  Texture2D sprite, glm::vec2 start_pos, float radius)
  : GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, radius), start_position_(start_pos)
{ 
  activated_ = false;
}

void BulletObject::Move(float dt, unsigned int window_width,unsigned int window_height)
{
  //Двигаем ядро
  position_ += velocity_*dt;  //Скорость содержит и направление ее обновляем в коллизиях
  //ПРоверка достижения границ экрана

  if(position_.x + size_.x <= 0.0f ||position_.x >= window_width)
  {
    Reset();
  }
   
  if(position_.y + size_.y <= 0.0f || position_.y >= window_height)
  {
    Reset();
  }
        
}

void  BulletObject::Reset()
{
  activated_ = false;
  position_ = start_position_;
  velocity_ = glm::vec2(0.0f);
}