#include "bullet_object.hpp"


  BulletObject::BulletObject(): GameObject(), StartPosition(0.0f){}

  BulletObject::BulletObject(glm::vec2 pos,  Texture2D sprite, glm::vec2 start_pos, float radius)
  : GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, radius), StartPosition(start_pos)
  { 
    this->Spawned = false;
  }

    glm::vec2 BulletObject::Move(float dt, unsigned int window_width,unsigned int window_height){

      //Двигаем ядро
        this->Position += this->Velocity*dt;  //Скорость содержит и направление ее обновляем в коллизиях

        //ПРоверка достижения границ экрана
        if(this->Position.x + this->Size.x <= 0.0f ||this->Position.x >= window_width)
        {
            this->Reset();
        }
   
        if(this->Position.y + this->Size.y <= 0.0f || this->Position.y >= window_height)
        {
           this->Reset();
        }
        
    
      return this->Position;  // TODO проверить этот возврат 

    }

    void  BulletObject::Reset(){

            this->Spawned = false;
            this->Position = this->StartPosition;
            this->Velocity = glm::vec2(0.0f);
     }