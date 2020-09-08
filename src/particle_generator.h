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

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"
#include "game_object.h"


// Represents a single particle and its state
struct Particle {
    glm::vec2 position, velocity;
    glm::vec4 color;
    float     life;

    Particle() : position(0.0f), velocity(0.0f), color(1.0f), life(0.0f) { }
};

const double kStartTime = 0.7;

class ParticleGenerator
{
public:
 
    ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount);
    void Update(float dt, GameObject &object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    void Draw();

    void set_work_time(double new_time = kStartTime);
private:

    std::vector<Particle> particles_;
    unsigned int amount_;
    unsigned int last_used_particle_;
    double work_time_;
   
    
    Shader shader_;
    Texture2D texture_;
    GLuint vao_;
   
    void Init();
    unsigned int FirstUnusedParticle();
    void RespawnParticle(Particle &particle, GameObject &object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};
