#include <math.h>
#include <raylib.h>

#define ARRAY_SIZE(array) sizeof(array) / sizeof(array[0])

#define PLAYER_TRAIL_LENGTH 5

#define PLAYER_WIDTH 50
#define PLAYER_HEIGHT 60

#define SPEED_INCREMENT 3
#define FRICTION 0.90
#define GRAVITY 1

#define COLLECTABLE_RADIUS 30

static Vector2 last_coords[PLAYER_TRAIL_LENGTH];
static int last_coords_count = 0;

#define ArrayCount(arr) (sizeof (arr) / sizeof *(arr))

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

    Vector2 velocity = {0};

    Vector2 collectables[3];
    for (int index = 0; index < ArrayCount(collectables); index += 1)
    {
        collectables[index].x = GetRandomValue(2 * COLLECTABLE_RADIUS, w - 2 * COLLECTABLE_RADIUS);
        collectables[index].y = GetRandomValue(2 * COLLECTABLE_RADIUS, h - 2 * COLLECTABLE_RADIUS);
    }

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        w = GetScreenWidth();
        h = GetScreenHeight();

        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) 
            velocity.x -= SPEED_INCREMENT;
        
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) 
            velocity.x += SPEED_INCREMENT;
        
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) 
            velocity.y -= SPEED_INCREMENT;
        
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) 
            velocity.y += SPEED_INCREMENT;

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

        last_coords[last_coords_count++] = (Vector2){ player.x, player.y };

        if (last_coords_count >= ARRAY_SIZE(last_coords))
            last_coords_count = 0;

        for (int index = 0; index < ArrayCount(collectables); index += 1)
        {
            if (CheckCollisionCircleRec(collectables[index], COLLECTABLE_RADIUS, player))
            {
                collectables[index].x = GetRandomValue(2 * COLLECTABLE_RADIUS, w - 2 * COLLECTABLE_RADIUS);
                collectables[index].y = GetRandomValue(2 * COLLECTABLE_RADIUS, h - 2 * COLLECTABLE_RADIUS);
            }
        }

        BeginDrawing();

        ClearBackground((Color) { 0, 0, 0, 255 });        
        
        for (int i = 0; i < ARRAY_SIZE(last_coords); i++)
        {
            Vector2 coords = last_coords[i];
            float radius = 10 + fabsf(sinf(GetTime())) * 5;

            DrawCircle(coords.x + player.width / 2, coords.y + player.height / 2, radius, (Color){ 0, 121, 241, 255 });
        }
        
        for (int index = 0; index < ArrayCount(collectables); index += 1)
        {
            Vector2 c = collectables[index];
            DrawCircle(c.x, c.y, COLLECTABLE_RADIUS, (Color) { 0xF4, 0xCA, 0x16, 0xFF });
        }

        // draw a bamboo hat
        DrawTriangle(
            (Vector2){ player.x + player.width / 2, player.y - 35 }, 
            (Vector2){ player.x - 35, player.y }, 
            (Vector2){ player.x + player.width + 35, player.y },
            (Color){ 226, 185, 143, 255 }
        );

        DrawRectangle(player.x, player.y, player.width, player.height, (Color) { 255, 255, 255, 255 });

        EndDrawing();
    }

    return 0;
}

