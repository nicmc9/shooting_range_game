#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <vector>
#include "sprite_renderer.h"
#include "game_level.h"

//основные игровые состояния
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};


class Game
{
public:
    GameState               State;
    bool                    Keys[1024];
    bool                    KeysProcessed[1024]; // Обработка нажатив для меню
    unsigned int            Width, Height;
 
        //контейнер уровней
    std::vector<GameLevel> Levels;
    unsigned int           CurrentLevel;
    //Заряды должны быть частью уровня, как и количество мишеней например и другие настройки
    unsigned int Сharges;
    //!Определить время игры
  
    SpriteRenderer* Renderer = nullptr;
        

    Game(unsigned int width, unsigned int height);
    ~Game();
     //Загружаем все шейдеры текстуры и создаем игровые объекты
    void Init();
    
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();

    // сброс параметров 
    void ResetLevel();
    void ResetPlayer();

    void DoCollisions();
};