#include <iostream>

#include <fstream>
#include <sstream>
#include <string>
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
        if (!target.Destroyed)  
            target.Draw(renderer);
}


//! Возможно замена на пустой объект
bool GameLevel::IsCompleted()
{
    for (GameObject &target : this->Targets)
        if (!target.Destroyed)
            return false;
    return true;
}

void GameLevel::init(std::vector<std::vector<unsigned int>> targetData, unsigned int levelWidth, unsigned int levelHeight)
{
    
    // Вычисляем размерность кирпичей в зависимости от размера экрана и их количества
    size_t height = targetData.size();
    size_t width = targetData[0].size(); 
    // мы можем индексировать вектор в [0], так как эта функция вызывается только если высота > 0
    float unit_width = levelWidth / static_cast<float>(width);
    float unit_height = levelHeight / static_cast<float>(height); 
   	
    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x) //width == 5
        {
    
            // проверяем тип блока и назначаем свойства
            if (targetData[y][x] == 1) // твердый
            {
               
                glm::vec2 pos(unit_width * x, unit_height * y);
                //TODO стратегия размеров - ?? должны быть разные но в пределах 

                glm::vec2 size(80, 50);
                GameObject obj(pos, size, ResourceManager::GetTexture("block"), glm::vec3(0.2f, 0.6f, 1.0f));
                this->Targets.push_back(obj);
            }
            else if (targetData[y][x] > 10)	// не твердый номер теперь означает цвет
            {
                //TODO Другие типы мишеней
            }
        }
    }
}