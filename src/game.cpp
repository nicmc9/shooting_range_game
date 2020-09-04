
#include <iostream>
#include "game.hpp"

#include "utility.hpp"

enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
}; 

using Collision = std::tuple<bool, Direction, glm::vec2>; 

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height), Сharges(20)
{
}

Game::~Game()
{
    delete Renderer;
    delete Player;
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
    ResourceManager::LoadTexture("resources/textures/Aim.png", true, "aim");


    //Загружаем все уровни
    GameLevel one; one.Load("resources/levels/one.lvl", this->Width, this->Height);
    this->Levels.push_back(one);

    this->CurrentLevel = 0;

    Renderer = new SpriteRenderer(sprite_shader);
    

    glm::vec2 playerPos = glm::vec2( this->Width / 2.0f - PLAYER_SIZE.x / 2.0f,  this->Height/2 - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("aim"));
}

void Game::Update(float dt )
{
    // В ходе игры мишень двигаеться и проверяет столкновение со стеной но не друг с другом
    this->Levels[this->CurrentLevel].Update(dt, this->Width, this->Height);
       
   //! необходимо закончить структуру загрузки уровня
   // проверка всех коллизий уже друг с другом
     this->DoCollisions();
}

void Game::ProcessInput(float dt)
{

}

 void Game::MouseInput(double xpos, double ypos)
 {
    glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f,  this->Height/2 - PLAYER_SIZE.y);
    this->Player->Position.x = xpos - this->Player->Size.x;
    this->Player->Position.y = ypos - this->Player->Size.y ;

    //ПРоверка достижения границ экрана
        if(this->Player->Position.x <= 0.0f)  
            this->Player->Position.x = 0.0f;
        else if (this->Player->Position.x + this->Player->Size.x >= this->Width)
            this->Player->Position.x = this->Width - this->Player->Size.x;
        else if(this->Player->Position.y <= 0.0f)
            this->Player->Position.y = 0.0f;
        else if(this->Player->Position.y + this->Player->Size.y >= this->Height)
            this->Player->Position.y = this->Height - this->Player->Size.y;
}


void Game::Render()
{
    // Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height));

    this->Player->Draw(*Renderer); 
    //Логигу отрисовки уровня отдаем самому уровню - вместе с настренным рисовальшиком
    // this->Levels[this->CurrentLevel].Draw(*Renderer);

}

void Game::ResetLevel()
{

}

void Game::ResetPlayer()
{

}
//Определение столкновений

Collision CheckCollision(GameObject &one, GameObject &two);
Direction VectorDirection(glm::vec2 target);


void Game::DoCollisions()
{

    auto size = this->Levels[this->CurrentLevel].Targets.size();

    for(int i = 0; i <size-1 ; i++)
    {
        GameObject& current = this->Levels[this->CurrentLevel].Targets[i];

        for(int j = i + 1; j < size; j++)
        {
            GameObject& ball = this->Levels[this->CurrentLevel].Targets[j];

            

            Collision collision = CheckCollision(current,ball);
          
            if(std::get<0>(collision)){
                   Direction dir = std::get<1>(collision);
                   glm::vec2 diff_vector  = std::get<2>(collision);

                    if(dir == LEFT || dir == RIGHT)
                   {
                        current.Velocity.x = -current.Velocity.x;

                        if(current.Velocity.x > 0 && ball.Velocity.x >0 ||current.Velocity.x < 0 && ball.Velocity.x <0)
                        { 
                            ball.Velocity.x = -ball.Velocity.x;
                        }
                        
                        float penetration = (current.Radius + ball.Radius) - std::abs(diff_vector.x);
                        if(dir == LEFT){
                            current.Position.x -= penetration;
                        }
                        else{
                            current.Position.x += penetration;
                          
                        }
                   }
                   else
                   {
                       current.Velocity.y = - current.Velocity.y;
                       if(current.Velocity.y > 0 && ball.Velocity.y >0 ||current.Velocity.y < 0 && ball.Velocity.y <0)
                        { 
                            ball.Velocity.y = -ball.Velocity.y;
                        }
                       float penetration = (current.Radius+ball.Radius)  - std::abs(diff_vector.y);
                       if(dir == UP){
                            current.Position.y += penetration;
                           
                       }
                       else{
                            current.Position.y -= penetration;
                            
                       }
                   }
            }

        }
    }

}


Collision CheckCollision(GameObject &one, GameObject &two) // AABB - Circle collision
{
    //получить точку центра шара
    glm::vec2 centerOne(one.Position + one.Radius);
    glm::vec2 centerTwo(two.Position + two.Radius);

    
    // получить вектор разницы между центрами
    glm::vec2 difference = centerOne - centerTwo;
    

    if(glm::length(difference) <= (one.Radius + two.Radius))
        return std::make_tuple(true, VectorDirection(difference), difference);
    else
        return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}   

Direction VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),	// up
        glm::vec2(1.0f, 0.0f),	// right
        glm::vec2(0.0f, -1.0f),	// down
        glm::vec2(-1.0f, 0.0f)	// left
    };
    float max = 0.0f;
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++)
    {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}    

