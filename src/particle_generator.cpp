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
    : shader_(shader), texture_(texture), amount_(amount),last_used_particle_(0)
{
    Init();
}


void ParticleGenerator::Update(float dt, GameObject &object, unsigned int new_particles, glm::vec2 offset)
{
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


// render all particles
void ParticleGenerator::Draw()
{
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
    print("random",random);
    float rColor = 0.5f + ((rand() % 100) / 100.0f);
    particle.position =  object.position_ + random + offset;
    particle.position.x += random;
    particle.color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.life = 1.0f;
    particle.velocity = glm::vec2(0.0, 50.0f);
}