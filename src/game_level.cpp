#include <iostream>

#include <fstream>
#include <sstream>
#include <string>
#include <random>  
#include <ctime> 


#include "game_level.h"

//!Возможно нужно убрать избыточный уровень в векторе
void GameLevel::Load(const char *file, unsigned int window_width, unsigned int window_height)
{
   
    targets_.clear();
    // загрузка из файла
    unsigned int target_code = 0;
    GameLevel level;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<unsigned int>> target_data;
    if (fstream)
    {
        while (std::getline(fstream, line)) // читаем по линиям из файла уровня
        {
            
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while (sstream >> target_code) {
                row.push_back(target_code);
            }
            target_data.push_back(row);
        
        }
        if (target_data.size() > 0)
           Init(target_data, window_width, window_height);
    }
}

//! Если будет удалять из памяти можно будет убрать проверку
void GameLevel::Draw(SpriteRenderer &renderer)
{
    for (GameObject &target : this->targets_)
        if (!target.destroyed_ && target.activated_)  
            target.Draw(renderer);
}
void GameLevel::Update(float dt, unsigned int window_width, unsigned int window_height )
{
   
    for(GameObject& target : this->targets_)
        if (!target.destroyed_ && target.activated_)   
            target.Move(dt, window_width, window_height);
}

//! Возможно замена на пустой объект
bool GameLevel::IsCompleted()
{
    for (GameObject &target : this->targets_)
        if (!target.destroyed_)
            return false;
    return true;
}

void GameLevel::Init(std::vector<std::vector<unsigned int>> target_data, unsigned int window_width, unsigned int window_height)
{
    
    // Вычисляем размерность кирпичей в зависимости от размера экрана и их количества
    size_t num_rows = target_data.size();
    size_t num_targets = target_data[0].size(); //! Здесь учитываеться что все строки в файле одинаковой длинны
    // мы можем индексировать вектор в [0], так как эта функция вызывается только если высота > 0

    float radius = 25.0f; 
    // точка появления
    
    auto posy =  0 + radius;
    //генератор случайных чисел новый
    std::mt19937 gen; 
    gen.seed(time(0));
    
    std::uniform_int_distribution<> velocityX(150, 300); 
    std::uniform_int_distribution<> velocityY(70, 150); 

    float unit_width = window_width / static_cast<float>(num_targets);
    
   

    for (unsigned int y = 0; y < num_rows; ++y)
    {
        for (unsigned int x = 0; x < num_targets; ++x) //width == 5
        {
                // проверяем тип блока и назначаем свойства
            if (target_data[y][x] == 1) // твердый
            {
               
                glm::vec2 pos(unit_width * x, posy);
                //TODO стратегия размеров - ?? должны быть разные но в пределах 
                glm::vec2 size(radius * 2.0f, radius * 2.0f);
                int velx = velocityX(gen);
                int vely = velocityY(gen);
                if(velx % 2 == 0 ) velx = -velx;
                glm::vec2 velocity(velx,vely);

                GameObject obj(pos, size, ResourceManager::GetTexture("target"), radius, glm::vec3(1.0f), velocity );
                

                this->targets_.push_back(obj);
                
            }
            else if (target_data[y][x] > 10)	// не твердый номер теперь означает цвет
            {
                //TODO Другие типы мишеней
            }
        }
    }
}