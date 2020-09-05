#pragma once

#include <vector>


#include <glad/glad.h>
#include <glm/glm.hpp>

 
#include "game_object.h"
#include "sprite_renderer.h"
#include "resource_manager.h"

#include "utility.hpp"



class BulletObject : public GameObject
{
public:
    glm::vec2 StartPosition;
      
    BulletObject();
    BulletObject(glm::vec2 pos,  Texture2D sprite, glm::vec2 start_pos, float radius = 15.0f);
    ~BulletObject() = default;

   glm::vec2 Move(float dt, unsigned int window_width,unsigned int window_height) override;

};


