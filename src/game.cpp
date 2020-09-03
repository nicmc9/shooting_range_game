
#include <iostream>
#include "game.hpp"


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
    ResourceManager::LoadTexture("resources/textures/block.png", false, "block");

    //Загружаем все уровни
    GameLevel one; one.Load("resources/levels/one.lvl", this->Width, this->Height / 2);
    this->Levels.push_back(one);
    this->CurrentLevel = 0;

    Renderer = new SpriteRenderer(sprite_shader);
  
}

void Game::Update(float dt)
{

}


void Game::ProcessInput(float dt)
{

}


void Game::Render()
{
   // Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f );

      //Логигу отрисовки уровня отдаем самому уровню - вместе с настренным рисовальшиком
       this->Levels[this->CurrentLevel].Draw(*Renderer);
}

void Game::ResetLevel()
{

}

void Game::ResetPlayer()
{
}

