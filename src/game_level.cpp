#include <iostream>

#include <fstream>
#include <sstream>
#include <string>
#include <random>  
#include <ctime> 


#include "game_level.h"

//!Возможно нужно убрать избыточный уровень в векторе
void GameLevel::Load(const char *file, unsigned int levelWidth, unsigned int levelHeight)
{
   
    this->Targets.clear();
    // загрузка из файла
    unsigned int targetCode = 0;
    GameLevel level;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<unsigned int>> targetData;
    if (fstream)
    {
        while (std::getline(fstream, line)) // читаем по линиям из файла уровня
        {
            
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while (sstream >> targetCode) {
                row.push_back(targetCode);
            }
            targetData.push_back(row);
        
        }
        if (targetData.size() > 0)
           this->init(targetData, levelWidth, levelHeight);
    }
}

//! Если будет удалять из памяти можно будет убрать проверку
void GameLevel::Draw(SpriteRenderer &renderer)
{
    for (GameObject &target : this->Targets)
        if (!target.destroyed_ && target.activated_)  
            target.Draw(renderer);
}
void GameLevel::Update(float dt, unsigned int window_width, unsigned int window_height )
{
   
    for(GameObject& target : this->Targets)
        if (!target.destroyed_ && target.activated_)   
            target.Move(dt, window_width, window_height);
}

//! Возможно замена на пустой объект
bool GameLevel::IsCompleted()
{
    for (GameObject &target : this->Targets)
        if (!target.destroyed_)
            return false;
    return true;
}

void GameLevel::init(std::vector<std::vector<unsigned int>> targetData, unsigned int levelWidth, unsigned int levelHeight)
{
    
    // Вычисляем размерность кирпичей в зависимости от размера экрана и их количества
    size_t numRows = targetData.size();
    size_t numTargets = targetData[0].size(); //! Здесь учитываеться что все строки в файле одинаковой длинны
    // мы можем индексировать вектор в [0], так как эта функция вызывается только если высота > 0

    float radius = 25.0f; 
    // точка появления
    
    auto posy =  0 + radius;
    //генератор случайных чисел новый
    std::mt19937 gen; 
    gen.seed(time(0));
    
    std::uniform_int_distribution<> velocityX(150, 300); 
    std::uniform_int_distribution<> velocityY(70, 150); 

    float unit_width = levelWidth / static_cast<float>(numTargets);
    
   

    for (unsigned int y = 0; y < numRows; ++y)
    {
        for (unsigned int x = 0; x < numTargets; ++x) //width == 5
        {
                // проверяем тип блока и назначаем свойства
            if (targetData[y][x] == 1) // твердый
            {
               
                glm::vec2 pos(unit_width * x, posy);
                //TODO стратегия размеров - ?? должны быть разные но в пределах 
                glm::vec2 size(radius * 2.0f, radius * 2.0f);
                int velx = velocityX(gen);
                int vely = velocityY(gen);
                if(velx % 2 == 0 ) velx = -velx;
                glm::vec2 velocity(velx,vely);

                GameObject obj(pos, size, ResourceManager::GetTexture("target"), radius, glm::vec3(1.0f), velocity );
                

                this->Targets.push_back(obj);
                
            }
            else if (targetData[y][x] > 10)	// не твердый номер теперь означает цвет
            {
                //TODO Другие типы мишеней
            }
        }
    }
}