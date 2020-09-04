#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <vector>
#include "sprite_renderer.h"
#include "game_level.h"
#include "game_object.h"

//основные игровые состояния
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

const glm::vec2 PLAYER_SIZE(50.0f, 50.0f);
const glm::vec2 STAND_SIZE(150.0f, 50.0f);
const glm::vec2 CANNON_SIZE(50.0f, 70.0f);

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
    GameObject*     Player = nullptr;
    GameObject*     Stand = nullptr;
    GameObject*     Cannon = nullptr;

    Game(unsigned int width, unsigned int height);
    ~Game();
     //Загружаем все шейдеры текстуры и создаем игровые объекты
    void Init();
    
    void ProcessInput(float dt);
    void MouseInput(double xpos, double ypos);
    void Update(float dt);
    void Render();

    // сброс параметров 
    void ResetLevel();
    void ResetPlayer();

    void DoCollisions();
};