/*******************************************************************
** This code is part of ShootingRange game.
** Created Baleyv Nikolay nicmc9@gmail.com
**
** ShootingRange is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "particle_generator.h"

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount)
    : shader_(shader), texture_(texture), amount_(amount), work_time_(kStartTime), last_used_particle_(0)
{
    Init();
}

//Функция делает запрос на создание новых частиц и обнобляет данные текущих

void ParticleGenerator::Update(float dt, GameObject &object, unsigned int new_particles, glm::vec2 offset)
{
   
       work_time_ -= dt;
        if(work_time_ < 0) return;

    // add new particles 
        for (unsigned int i = 0; i < new_particles; ++i)
        {
            int unused_particle = FirstUnusedParticle();
            RespawnParticle(particles_[unused_particle], object, offset);
        }
    // update all particles
        for (unsigned int i = 0; i < this->amount_; ++i)
        {
            Particle &p = particles_[i];
            p.life -= dt; // уменьшаем жизнь
            if (p.life > 0.0f)
            {	
               // если жива обновляем позицию и делаем прозрачнее
                p.position -= p.velocity * dt; 
                p.color.a -= dt * 1.0f;
            }
        }
}


void ParticleGenerator::set_work_time(double new_time)
{
    work_time_ = new_time;
}

// отрисовка новых частиц
void ParticleGenerator::Draw()
{
    if(work_time_ < 0) return; 

    shader_.Use();
    for (auto& particle : particles_)
    {
       if (particle.life > 0.0f)
           {
            shader_.SetVector2f("offset", particle.position);
            shader_.SetVector4f("color", particle.color);
            texture_.Bind();
            glBindVertexArray(vao_);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
           }
    }
}
// инициализация  происходит во время загрузки игры
void ParticleGenerator::Init()
{
    // настраиваем вершинный буфер
    unsigned int vbo;
    float particle_quad[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    }; 

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao_);
    // fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    // set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindVertexArray(0);

    //при загрузке сразу создаем все объекты частиц
    //чтобы в  работе не выделять память
    for (unsigned int i = 0; i < amount_; ++i)
        particles_.push_back(Particle());
}

// функция возвращаем индекс частицы которую мы будем возродать
unsigned int ParticleGenerator::FirstUnusedParticle()
{
    // сначала ище от предыдущего индекса
    for (unsigned int i = last_used_particle_; i < amount_; ++i){
        if (particles_[i].life <= 0.0f){
            last_used_particle_ = i;
            return i;
        }
    }
    // если не вышло то используем поиск от начала
    for (unsigned int i = 0; i < last_used_particle_; ++i){
        if (particles_[i].life <= 0.0f){
            last_used_particle_ = i;
            return i;
        }
    }
    // если не вышло отдаем просто первую частицу , хотя эта ситуация нежелательна 
    last_used_particle_ = 0;
    return 0;
}

void ParticleGenerator::RespawnParticle(Particle &particle, GameObject &object, glm::vec2 offset)
{
    float random = ((rand() % 100) - 50) / 10.0f;
    float rColor = 0.5f + ((rand() % 100) / 100.0f);

    particle.color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.life = 1.0f;
    particle.velocity = glm::vec2(0,150);
  
    particle.position = glm::vec2(0.0f,0.0f); // без этого следующие 500 частиц получают сумму и улетают

    glm::mat4 model = glm::mat4(1.0f);
    glm::vec2 origin =  glm::vec2(0.5f,0.67f);

    // выравниваем дым в зависимости от угла повората 10 это scale/2 в scale в шейдере
    float offset_smoke_x = object.size_.x;
    float offset_smoke_y = 0.0f;
    if(object.rotation_ < -40.0f)
        offset_smoke_x = object.size_.x/2.0f+10;
    else
        offset_smoke_x = object.size_.x/2.0f-10;

    if(object.rotation_ > 40.0f)
           offset_smoke_y  =10.0f;
    
    model = glm::translate(model, glm::vec3(object.position_.x,object.position_.y,  0.0f));
    model = glm::translate(model, glm::vec3(origin.x * object.size_.x, origin.y * object.size_.y, 0.0f)); // возвращаем обратно
    model = glm::rotate(model, glm::radians(object.rotation_), glm::vec3(0.0f, 0.0f, 1.0f)); //поворачиваем 
    model = glm::translate(model, glm::vec3(-origin.x * object.size_.x , -origin.y * object.size_.y, 0.0f)); //переность ось вращения к ценру квадрата 
    model = glm::translate(model, glm::vec3(offset_smoke_x ,offset_smoke_y ,  0.0f)); 

    particle.position =  model * glm::vec4(particle.position.x, particle.position.y , 0.0f, 1.0f);

    particle.position.x  +=   random + offset.x;
    particle.position.y  +=   random + offset.y; 
}