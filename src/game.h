#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <vector>
#include "sprite_renderer.h"
#include "game_level.h"
#include "game_object.h"
#include "bullet_object.h"
#include "text_renderer.h"

//основные игровые состояния
enum class GameState {
    kGameActive,
    kGameMenu,
    kGameWin,
    kGameOver
};



const glm::vec2 kPlayerSize(50.0f, 50.0f);
const glm::vec2 kStandSize(150.0f, 50.0f);
const glm::vec2 kCannonSize(50.0f, 70.0f);
const glm::vec2 kBulletSize(30.0f, 30.0f);
const float     kBulletStreight{450.0f};

class Game
{
public:
    Game(unsigned int width, unsigned int height);
    ~Game();

    enum class Direction { kUp, kRight, kDown, kLeft }; 
    using Collision = std::tuple<bool, Direction, glm::vec2>; 
    
    auto& targets();
    void set_state(GameState state);
    void set_keys(int key, bool state);
    void set_keys_processed(int key, bool state);

    //!Определить время игры
  
    SpriteRenderer* Renderer = nullptr;
    GameObject*     Player = nullptr;
    GameObject*     Stand = nullptr;
    GameObject*     Cannon = nullptr;
    TextRenderer*   Text = nullptr;

   
     //Загружаем все шейдеры текстуры и создаем игровые объекты
    void Init();
    void ProcessInput(float dt);
    void MouseInput(double xpos, double ypos);
    void MouseButtonClick();
    void Update(float dt);
    void Render();

private:
    
    // сброс параметров 
    void ResetLevel();
    void ResetPlayer();
    void DoCollisions();

private:
    std::vector<GameLevel> levels_;
    unsigned int           current_level_;
    glm::vec2              cannon_down_point_;
    float                  cannon_reload_time_;

    GameState               state_;
    bool                    keys_[1024];
    bool                    keys_processed_[1024]; // Обработка нажатив для меню
    unsigned int            screen_width_, screen_height_;
 
    std::vector<BulletObject> cannon_balls_;
};