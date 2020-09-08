#include <iostream>
#include <algorithm>

#include "game.h"
#include "utility.h"

#include <irrKlang.h>
using namespace irrklang;

ISoundEngine *SoundEngine = createIrrKlangDevice();

Game::Game(unsigned int width, unsigned int height)
    : state_(GameState::kGameActive), keys_(), screen_width_(width), screen_height_(height)
{
    
}

Game::~Game()
{
    delete Renderer;
    delete Player;
    delete Stand;
    delete Cannon;
    delete Text;
    delete Clock;
    delete Particles;
    SoundEngine->drop();
}

///getters and setter

auto& Game::targets()   {    return levels_[current_level_].targets_;   }
void Game::set_state(GameState state)    {       state_ = state;       }
void Game::set_keys(int key, bool state) {       keys_[key]  = state;  }
void Game::set_keys_processed(int key, bool state) {       keys_processed_[key]  = state;  }


void Game::Init()
{
    Shader sprite_shader = ResourceManager::LoadShader("resources/shaders/sprite.vs", "resources/shaders/sprite.fs", nullptr, "sprite");
    Shader particle_shader = ResourceManager::LoadShader("resources/shaders/particle.vs", "resources/shaders/particle.fs", nullptr, "particle");

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(screen_width_), static_cast<float>(screen_height_), 0.0f, -1.0f, 1.0f);
    //uniform sampler2D image; индекс в наборе текстур для данного семплера, т.к всегда одна текстура ставим это здесь
    sprite_shader.Use().SetInteger("image", 0);
    sprite_shader.SetMatrix4("projection", projection);

    particle_shader.Use().SetInteger("sprite", 0);
    particle_shader.SetMatrix4("projection", projection);    

    //Загружаем все текстуры
    ResourceManager::LoadTexture("resources/textures/background.png", false, "background");
    ResourceManager::LoadTexture("resources/textures/TargetP.png", true, "targetp");
    ResourceManager::LoadTexture("resources/textures/TargetG.png", true, "targetg");
    ResourceManager::LoadTexture("resources/textures/Aim.png", true, "aim");
    ResourceManager::LoadTexture("resources/textures/Stand.png", true, "stand");
    ResourceManager::LoadTexture("resources/textures/Cannon.png", true, "cannon");
    ResourceManager::LoadTexture("resources/textures/Cannonball.png", true, "cannonball");
    ResourceManager::LoadTexture("resources/textures/Clock.png", true, "clock");
    ResourceManager::LoadTexture("resources/textures/smoke2.png", true, "particle");

    Text = new TextRenderer(screen_width_, screen_height_);
    Text->Load("resources/fonts/Grandstander.ttf", 24);

    //Загружаем все уровни
    GameLevel one; one.Load("resources/levels/one.lvl", screen_width_, screen_height_);
    GameLevel two; two.Load("resources/levels/two.lvl", screen_width_, screen_height_);
    levels_.push_back(one);
    levels_.push_back(two);
    current_level_ = 0;

    Renderer = new SpriteRenderer(sprite_shader);
    
    //Пушка и подставка    
    glm::vec2 stand_pos = glm::vec2(screen_width_ / 2.0f - kStandSize.x / 2.0f,screen_height_ - kStandSize.y);
    Stand = new GameObject(stand_pos, kStandSize, ResourceManager::GetTexture("stand"));

    glm::vec2 cannon_pos = glm::vec2(screen_width_ / 2.0f - kCannonSize.x / 2.0f, screen_height_ - kCannonSize.y - kStandSize.y/2);
    Cannon = new GameObject(cannon_pos, kCannonSize, ResourceManager::GetTexture("cannon"));
    cannon_down_point_ = glm::vec2(screen_width_ / 2.0f,  cannon_pos.y + kCannonSize.y);
    cannon_reload_time_ = 0.0f; // 0 Можно стрелять после выстрела  увеличиваем

    glm::vec2 player_pos = glm::vec2(screen_width_ / 2.0f - kPlayerSize.x / 2.0f,  screen_height_/2 - kPlayerSize.y);
    Player = new GameObject(player_pos, kPlayerSize, ResourceManager::GetTexture("aim"), kPlayerSize.x/2);

    //пока сделаем 3 снаряда    

    glm::vec2 bullet_pos = glm::vec2(screen_width_ / 2.0f - kBulletSize.x / 2.0f,  screen_height_ - kBulletSize.y- kStandSize.y/2);
    //Пока делаем 3 возможных снаряда
    for(int i = 0; i < 3 ; i++){
        cannon_balls_.emplace_back(BulletObject(bullet_pos,  ResourceManager::GetTexture("cannonball"), bullet_pos, kBulletSize.x/2));
    }

    //Данные времен и объект часов    
    start_time_ = 0.0;
    duration_ =  0.0;

    Clock = new GameObject(glm::vec2(20.0f,10.0f), glm::vec2(40.0f,40.0f), ResourceManager::GetTexture("clock"));

    // счетчик сбитых мишеней
    downs_targets_  = 0;
    Particles = new ParticleGenerator( particle_shader,  ResourceManager::GetTexture("particle"),  500  );

    //SoundEngine->play2D("resources/audio/fone.mp3", true);
}

void Game::Update(float dt )
{
    // В ходе игры мишень двигаеться и проверяет столкновение со стеной но не друг с другом
    levels_[current_level_].Update(dt, screen_width_, screen_height_);

   for(auto& cannon_ball : cannon_balls_)
        if (!cannon_ball.destroyed_ && cannon_ball.activated_)  
            cannon_ball.Move(dt, screen_width_, screen_height_);

    //Обновляем возможность выстрела
    cannon_reload_time_ -= dt;

    if(cannon_reload_time_ < 0) cannon_reload_time_ = 0.0f; //TODO  изменить после расчета столкновений со снарядом

   //обновление частиц  2 == newParticles
    Particles->Update(dt, *Cannon, 2);

   // проверка всех коллизий уже друг с другом
   // Удаляем все уничтоженные цели 

    auto destroyed = [](const auto& target) { return target.destroyed_; };
    targets().erase(std::remove_if(targets().begin(), targets().end(), destroyed), targets().end());

    if (state_ == GameState::kGameActive && targets().size()==0)
    {
        ResetLevel();
        ResetPlayer();
        state_ = GameState::kGameWin;
    }
 
    DoCollisions();

    if (state_ == GameState::kGameActive)
    {
        if(IsGameOver()) {
        
             ResetLevel();
             ResetPlayer();
             state_ = GameState::kGameOver;
        }
    }
}

 void Game::StartLevelTime(double duration){
     start_time_ = glfwGetTime();
     duration_ = duration;
     timer_ = std::to_string(static_cast<int>(duration_));
 }

 bool Game::IsGameOver(){
     
     double current_time = glfwGetTime();
     double elapsed_time = current_time - start_time_;
     double diff = duration_ - elapsed_time;   
     timer_ = std::to_string(static_cast<int>(diff));

     if(elapsed_time > duration_) return true;

     return false;
 }


void Game::ProcessInput(float dt)
{

if (state_ == GameState::kGameMenu)
{
    if (keys_[GLFW_KEY_ENTER] && !keys_processed_[GLFW_KEY_ENTER])
        {
            state_ = GameState::kGameActive;
            keys_processed_[GLFW_KEY_ENTER] = true;
            StartLevelTime( levels_[current_level_].level_time_ );  //!здесь нужно получить переменную из уровня
        }
    if (keys_[GLFW_KEY_W] && !keys_processed_[GLFW_KEY_W])
        {
           current_level_ = (current_level_ + 1) % 2; //Этого достаточно чтобы поменять отрисовку уровня 
           keys_processed_[GLFW_KEY_W] = true;
        }
    if (keys_[GLFW_KEY_S] && !keys_processed_[GLFW_KEY_S])
        {
           if (current_level_ > 0)
            --current_level_;
           else
            current_level_ = 1;
            keys_processed_[GLFW_KEY_S] = true;
        }
}  

if (state_ == GameState::kGameWin)
    {
        if (keys_[GLFW_KEY_ENTER])
        {
            keys_processed_[GLFW_KEY_ENTER] = true; //Иначе сразу запустит уровень, без входа в меню
            state_ = GameState::kGameMenu;
        }
    }    

  if (state_ == GameState::kGameOver)
    {
        if (keys_[GLFW_KEY_ENTER])
        {
            keys_processed_[GLFW_KEY_ENTER] = true; //Иначе сразу запустит уровень, без входа в меню
            state_ = GameState::kGameMenu;
        }
    }      

}

void Game::MouseInput(double xpos, double ypos)
{
    glm::vec2 new_pos = glm::vec2(xpos - Player->size_.x/2,ypos - Player->size_.y/2 );
    Player->set_position_bound(new_pos, screen_width_, screen_height_ );

    //Обновляем также поворот пушки т.к. это имеет значение только при движениях мыши
    glm::vec2 up = glm::vec2(0.0f, -1.0f);
 
    glm::vec2 player_centre = glm::vec2(Player->position_.x + (Player->size_.x/2) , Player->position_.y + (Player->size_.y/2) );
    glm::vec2 direction = glm::normalize(player_centre - cannon_down_point_);
       
    //atan2(AxBy - BxAy, AxBx + AyBy) упрощение для двумерного случая
    //сокращаем т.к. up.x == 0 : glm::atan(direction.x * up.y - up.x * direction.y, direction.x * up.x+direction.y * up.y);
    float ungle =  glm::atan(direction.x * up.y, direction.y * up.y);
   
    ungle = -glm::degrees(ungle); //TODO знак нужен для трансформации рендера , продумать еще раз 
    //print("ungle", ungle);
    Cannon->rotation_ = ungle;
}

void Game::MouseButtonClick(){

if (state_ == GameState::kGameActive)
    {
       if(cannon_reload_time_ > 0) return;

        //Particles->set_work_time(); // пока время по умолчанию

        for(auto& cannon_ball : cannon_balls_){
            if (!cannon_ball.activated_) {
                
               SoundEngine->play2D("resources/audio/shut.mp3", false);
               cannon_ball.velocity_ = glm::normalize(Player->position_ - cannon_ball.start_position_ )* kBulletStreight;
               cannon_ball.activated_ = true;
               cannon_reload_time_ = 1.0; // выстрел ставим время 
               break;      
          } 
       }
   }
}

void Game::Render()
{
    if(state_ == GameState::kGameActive|| state_ == GameState::kGameMenu || state_ == GameState::kGameWin|| state_ == GameState::kGameOver)
    {
    //Порядок отрисовки не забывай
    Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(screen_width_,screen_height_));
    
    //Логигу отрисовки уровня отдаем самому уровню - вместе с настренным рисовальшиком
    levels_[current_level_].Draw(*Renderer);
    
    for(auto& cannon_ball : cannon_balls_)
        if (!cannon_ball.destroyed_ && cannon_ball.activated_)  
            cannon_ball.Draw(*Renderer);
    
    Stand->Draw(*Renderer); 
    Cannon->Draw(*Renderer,glm::vec2(0.5f,0.67f));      
    Particles->Draw();
  
    Player->Draw(*Renderer);

    Clock->Draw(*Renderer); 
    Text->RenderText(timer_, 55.0f, 20.0f, 1.0f, glm::vec3(1.0f, 0.0f, 1.0f));

    Text->RenderText(std::to_string(downs_targets_), (screen_width_ - 50), 20.0f, 1.0f, glm::vec3(1.0f, 0.0f, 1.0f));

    }

      if (state_ == GameState::kGameMenu)
    {
        Text->RenderText("Press ENTER to start", 380.0f, screen_height_ / 2, 1.0,   glm::vec3(1.0f, 0.0f, 1.0f));
        Text->RenderText("Press W or S to select level", 330.0f, screen_height_ / 2 + 20.0f, 1.0, glm::vec3(1.0f, 0.0f, 1.0f) );
    }

     if (state_ == GameState::kGameWin)
    {
        Text->RenderText("You WON!!!", 420.0, screen_height_ / 2 - 20.0, 1.0, glm::vec3(1.0f, 0.0f, 1.0f)  );
        Text->RenderText("Press ENTER to retry or ESC to quit", 280.0, screen_height_ / 2, 1.0, glm::vec3(1.0f, 0.0f, 1.0f));
    }     

     if (state_ == GameState::kGameOver)
    {
        Text->RenderText("GAME OVER!!!", 460.0, screen_height_ / 2 - 20.0, 1.0, glm::vec3(1.0f, 0.0f, 1.0f)  );
        Text->RenderText("Press ENTER to retry or ESC to quit", 280.0, screen_height_ / 2, 1.0, glm::vec3(1.0f, 0.0f, 1.0f));
    }     
}

//TODO зависимость от количества уровней, можно испровить позже
void Game::ResetLevel()
{
    timer_ = "";
    downs_targets_ = 0;

    if (current_level_ == 0)
        levels_[0].Load("resources/levels/one.lvl", screen_width_, screen_height_);
    else if (current_level_ == 1)
        levels_[1].Load("resources/levels/two.lvl", screen_width_, screen_height_);
}

void Game::ResetPlayer()
{

}
//Определение столкновений
//TODO сделать статическими
Game::Collision CheckCollision(GameObject &one, GameObject &two);
Game::Direction VectorDirection(glm::vec2 target);


void Game::DoCollisions()
{
     //проверка коллизий между целями
    auto size = targets().size();

    if(size == 0) return; 

    for(int i = 0; i < size-1 ; i++)
    {
        auto& current_aim = targets()[i];

        for(int j = i + 1; j < size; j++)
        {
            auto& next_aim = targets()[j];
            //Collision collision = CheckCollision(current_aim, next_aim);
            auto [is_collision, direction, diff_vector] = CheckCollision(current_aim, next_aim);

            if(is_collision){
                   if(direction == Direction::kLeft || direction == Direction::kRight)
                   {
                        current_aim.velocity_.x =  -current_aim.velocity_.x;
            
                        if(current_aim.velocity_.x  > 0 && next_aim.velocity_.x  >0 ||current_aim.velocity_.x  < 0 && next_aim.velocity_.x  <0)
                        { 
                            next_aim.velocity_.x  = -next_aim.velocity_.x;
                        }
                        
                        float penetration = (current_aim.radius_ + next_aim.radius_) - std::abs(diff_vector.x);

                        if(direction == Direction::kLeft)
                           current_aim.position_.x -= penetration;
                        else
                           current_aim.position_.x += penetration;
                    }
                   else
                   {
                       current_aim.velocity_.y = -current_aim.velocity_.y;
                   
                       if(current_aim.velocity_.y > 0 && next_aim.velocity_.y >0 ||current_aim.velocity_.y < 0 && next_aim.velocity_.y <0)
                        { 
                            next_aim.velocity_.y = -next_aim.velocity_.y;
                        }
                       
                       float penetration = (current_aim.radius_+next_aim.radius_)  - std::abs(diff_vector.y);

                       if(direction == Direction::kUp)
                          current_aim.position_.y += penetration;
                       else
                          current_aim.position_.y -= penetration;
                       
                   }
            }

        }
    }

    //проверка коллизий с ядром

    for(auto& cannon_ball: cannon_balls_){
           if(cannon_ball.activated_){
               for(auto& target: targets()){

                Collision collision = CheckCollision(cannon_ball, target);
                    if(std::get<0>(collision)){  //TODO Добавить звуки
                        cannon_ball.Reset();

                        SoundEngine->play2D("resources/audio/boom.mp3", false);

                        target.health_ -= 1.0f;
                        target.color_ -= glm::vec3(0.0f,0.1f,0.2f);

                        if(target.health_ <= 0){
                            downs_targets_ += 1;
                            target.destroyed_ = true;
                        }
                    }
               }
           } 
    }
}


Game::Collision CheckCollision(GameObject &one, GameObject &two) // AABB - Circle collision
{
    //получить точку центра шара
    glm::vec2 centerOne(one.position_ + one.radius_);
    glm::vec2 centerTwo(two.position_ + two.radius_);
   
    // получить вектор разницы между центрами
    glm::vec2 difference = centerOne - centerTwo;
    
    if(glm::length(difference) <= (one.radius_ + two.radius_))
        return std::make_tuple(true, VectorDirection(difference), difference);
    else
        return std::make_tuple(false, Game::Direction::kUp, glm::vec2(0.0f, 0.0f));
}   

Game::Direction VectorDirection(glm::vec2 target)
{
    //TODO еще раз проверить векторы
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
    return (Game::Direction)best_match;
}    

