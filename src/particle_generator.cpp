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
    : shader_(shader), texture_(texture), amount_(amount), work_time_(kStartTime)
{
    Init();
}


void ParticleGenerator::Update(float dt, GameObject &object, unsigned int new_particles, glm::vec2 offset)
{
   
    //work_time_ -= dt;
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
            p.life -= dt; // reduce life
            if (p.life > 0.0f)
            {	// particle is alive, thus update
               
                p.position -= p.velocity * dt; 
                p.color.a -= dt * 1.0f;
            }
        }
    
}

//TODO возможно добавить проверку ошибок
void ParticleGenerator::set_work_time(double new_time){
    work_time_ = new_time;
    //print(new_time);
}

// render all particles
void ParticleGenerator::Draw()
{
    //if(work_time_ < 0) return; 
   // use additive blending to give it a 'glow' effect
   // glBlendFunc(GL_SRC_ALPHA, GL_ONE);
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
    
  //  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::Init()
{
    // set up mesh and attribute properties
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

    // create this->amount default particle instances
    for (unsigned int i = 0; i < amount_; ++i)
        particles_.push_back(Particle());
}

// stores the index of the last particle used (for quick access to next dead particle)
unsigned int last_used_particle_ = 0;
unsigned int ParticleGenerator::FirstUnusedParticle()
{
    // first search from last used particle, this will usually return almost instantly
    for (unsigned int i = last_used_particle_; i < amount_; ++i){
        if (particles_[i].life <= 0.0f){
            last_used_particle_ = i;
            return i;
        }
    }
    // otherwise, do a linear search
    for (unsigned int i = 0; i < last_used_particle_; ++i){
        if (particles_[i].life <= 0.0f){
            last_used_particle_ = i;
            return i;
        }
    }
    // all particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
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

    model = glm::translate(model, glm::vec3(object.position_.x,object.position_.y,  0.0f));

    model = glm::translate(model, glm::vec3(origin.x * object.size_.x, origin.y * object.size_.y, 0.0f)); // возвращаем обратно
    model = glm::rotate(model, glm::radians(object.rotation_), glm::vec3(0.0f, 0.0f, 1.0f)); //поворачиваем 
    model = glm::translate(model, glm::vec3(-origin.x * object.size_.x, -origin.y * object.size_.y, 0.0f)); //переность ось вращения к ценру квадрата 
    model = glm::translate(model, glm::vec3(object.size_.x/2.0f -10 ,0.0f,  0.0f)); //10 -scale
    model = glm::scale(model, glm::vec3(object.size_ , 1.0f)); // масштабируем

    particle.position =  model * glm::vec4(particle.position.x, particle.position.y , 0.0f, 1.0f);

    particle.position.x  +=   random + offset.x;
    particle.position.y  +=   random + offset.y; 

    //print(particle.position.x,particle.position.y);
}