
#include <iostream>
#include "game.hpp"


bool CheckCollision(GameObject& one, GameObject& two);

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height), Сharges(20)
{
}

Game::~Game()
{
    delete Renderer;
}

void Game::Init()
{
    Shader sprite_shader = ResourceManager::LoadShader("resources/shaders/sprite.vs", "resources/shaders/sprite.fs", nullptr, "sprite");
   
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    //uniform sampler2D image; индекс в наборе текстур для данного семплера, т.к всегда одна текстура ставим это здесь
    sprite_shader.Use().SetInteger("image", 0);
    //матрица проекции одинаковая для всех спрайтов в игре поэтому ставим здесь
    sprite_shader.SetMatrix4("projection", projection);

    //Загружаем все текстуры
    ResourceManager::LoadTexture("resources/textures/background.png", false, "background");
    ResourceManager::LoadTexture("resources/textures/Target.png", true, "target");

    //Загружаем все уровни
    GameLevel one; one.Load("resources/levels/one.lvl", this->Width, this->Height);
    this->Levels.push_back(one);
    this->CurrentLevel = 0;

    Renderer = new SpriteRenderer(sprite_shader);
  
}

void Game::Update(float dt)
{
    for(GameObject& ball : this->Levels[this->CurrentLevel].Targets)
         ball.Move(dt, this->Width,this->Height);
    

   // проверка всех коллизий
    //  this->DoCollisions();

}

void Game::DoCollisions()
{


     for(GameObject& box1 : this->Levels[this->CurrentLevel].Targets)
    {
         if(box1.Position.x <=0 || (box1.Position.x + box1.Size.x) >= this->Width){
              box1.Velocity.x = -box1.Velocity.x;
         } 
          if(box1.Position.y <=0 ||(box1.Position.y + box1.Size.y) >= this->Height){
              box1.Velocity.y = -box1.Velocity.y;
         } 
    }


}

void Game::ProcessInput(float dt)
{

}


void Game::Render()
{
      // Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height));

      //Логигу отрисовки уровня отдаем самому уровню - вместе с настренным рисовальшиком
       this->Levels[this->CurrentLevel].Draw(*Renderer);
}

void Game::ResetLevel()
{

}

void Game::ResetPlayer()
{
}





 //AABB collision
bool CheckCollision(GameObject& one, GameObject& two){

    // collision x -axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x  && two.Position.x + two.Size.x >=one.Position.x;
    //collison  y - axis? 
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y && two.Position.y + two.Size.y >= one.Position.y;
    //
    return collisionX&&collisionY;
}

