#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

 
#include "game_object.h"
#include "sprite_renderer.h"
#include "resource_manager.h"

#include "utility.h"



class BulletObject : public GameObject
{
public:
   BulletObject();
   BulletObject(glm::vec2 pos,  Texture2D sprite, glm::vec2 start_pos, float radius = 15.0f);
   virtual ~BulletObject() = default;

   virtual void Move(float dt, unsigned int window_width,unsigned int window_height) override;
   void         Reset();

 

public:
   glm::vec2 start_position_;
};


