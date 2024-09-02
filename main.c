#include <raylib.h>

#define PLAYER_WIDTH 50
#define PLAYER_HEIGHT 60
#define SPEED_INCREMENT 3
#define FRICTION 0.90
#define GRAVITY 1

typedef struct {
  float x;
  float y;
} Velocity;

int main()
{
    SetTraceLogLevel(LOG_NONE);
        
    InitWindow(0, 0, "game");
    ToggleFullscreen();

    int w = GetScreenWidth();
    int h = GetScreenHeight();
    
    Rectangle player = { .width = PLAYER_WIDTH, .height = PLAYER_HEIGHT };
    player.x = (w - player.width) / 2;
    player.y = (h - player.height) / 2;

    Velocity velocity = {0};

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        w = GetScreenWidth();
        h = GetScreenHeight();

        if (IsKeyDown(KEY_LEFT)) velocity.x -= SPEED_INCREMENT;
        if (IsKeyDown(KEY_RIGHT)) velocity.x += SPEED_INCREMENT;
        if (IsKeyDown(KEY_UP)) velocity.y -= SPEED_INCREMENT;
        if (IsKeyDown(KEY_DOWN)) velocity.y += SPEED_INCREMENT;

        player.x += (int)velocity.x;
        player.y += (int)velocity.y;
        if (player.x < 0) {
          player.x = 0;
          velocity.x = 0;
        }
        if (player.y < 0) {
          player.y = 0;
          velocity.y = 0;
        }
        if (player.x > (w-PLAYER_WIDTH)) {
          player.x = (w-PLAYER_WIDTH);
          velocity.x = 0;
        }
        if (player.y > (h-PLAYER_HEIGHT)) {
          player.y = h-PLAYER_HEIGHT;
          velocity.y = 0;
        }

        velocity.x *= FRICTION;
        velocity.y += GRAVITY;

        BeginDrawing();

        ClearBackground((Color) { 0, 0, 0, 255 });

        DrawRectangle(player.x, player.y, player.width, player.height, (Color) { 255, 255, 255, 255 });
        
        EndDrawing();
    }

    return 0;
}

