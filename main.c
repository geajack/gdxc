#include <raylib.h>

int main()
{
    SetTraceLogLevel(LOG_NONE);
        
    InitWindow(0, 0, "game");
    ToggleFullscreen();

    int w = GetScreenWidth();
    int h = GetScreenHeight();
    
    Rectangle player = { .width = 50, .height = 60 };
    player.x = (w - player.width) / 2;
    player.y = (h - player.height) / 2;

    int speed = 20;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        w = GetScreenWidth();
        h = GetScreenHeight();

        if (IsKeyDown(KEY_LEFT)) player.x -= speed;
        if (IsKeyDown(KEY_RIGHT)) player.x += speed;
        if (IsKeyDown(KEY_UP)) player.y -= speed;
        if (IsKeyDown(KEY_DOWN)) player.y += speed;

        if (player.x < 0) player.x = 0;
        if (player.x > w - player.width) player.x = w - player.width;
        if (player.y < 0) player.y = 0;
        if (player.y > h - player.height) player.y = h - player.height;

        BeginDrawing();

        ClearBackground((Color) { 0, 0, 0, 255 });

        DrawRectangle(player.x, player.y, player.width, player.height, (Color) { 255, 255, 255, 255 });
        
        EndDrawing();
    }

    return 0;
}
