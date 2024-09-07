
#include <stdlib.h>
#include <string.h>

#include <math.h>
#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>

#define ARRAY_SIZE(array) sizeof(array) / sizeof(array[0])

#define PLAYER_TRAIL_LENGTH 5

#define PLAYER_WIDTH 50
#define PLAYER_HEIGHT 60

#define SPEED_LIMIT 30
#define X_SPEED_INCREMENT 2
#define JUMP_SPEED 20
#define JUMP_LIMIT 2 // double, triple jump...

#define FRICTION 0.75
#define GRAVITY 1

#define COLLECTABLE_RADIUS 30

static Vector2 last_coords[PLAYER_TRAIL_LENGTH];
static int last_coords_count = 0;

static int debug_enabled = 0;
#define KEY_DEBUG_TOGGLE KEY_F1

#define BAMBOO_CAP 12
#define BAMBOO_THICC 12
#define BAMBOO_COLOR GetColor(0x7fa643ff)
Vector2 bamboo[BAMBOO_CAP];
float bamboo_tilt[BAMBOO_CAP];
float bamboo_wobble[BAMBOO_CAP];
float bamboo_width = 5.0f;

////HUD Settings
////////////////
#define COMBO_DROP_TIME 2.0f //In seconds
#define COMBO_BAR_WIDTH 120
#define COMBO_BAR_HEIGHT 20
#define COMBO_BAR_PADDING 4 //Padding from edge of screen
#define COMBO_BAR_FULL_COLOR (Color){0,255,0,255}
#define COMBO_BAR_EMPTY_COLOR (Color){128,0,0,255}
int player_score = 0;
int combo_mult = 1;
float combo_drop_timer = 0.f;

#define SCORE_FONT_SIZE 24
#define SCORE_PADDING 4 //Padding from edge of screen

#define COMBO_FONT_SIZE 16
#define COMBO_PADDING 8 //Padding from edge of screen

////////////////////

#define GUN_WIDTH (PLAYER_WIDTH * 0.2f)
#define GUN_HEIGHT (PLAYER_HEIGHT * 0.8f)

#define MAX_BULLETS_ON_SCREEN 64
#define BULLET_SPEED 500 /* pixels per second */
#define BULLET_RADIUS 16
struct bullet
{
    Vector2 pos, dir;
};


const Color ColorLerp(const Color c1, const Color c2, const float t);
void bamboo_generate(int w, int h);

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
    int player_jump_count = 0;

    Vector2 velocity = {0};

    Vector2 collectables[3];
    for (int index = 0; index < ARRAY_SIZE(collectables); index += 1)
    {
        collectables[index].x = GetRandomValue(2 * COLLECTABLE_RADIUS, w - 2 * COLLECTABLE_RADIUS);
        collectables[index].y = GetRandomValue(2 * COLLECTABLE_RADIUS, h - 2 * COLLECTABLE_RADIUS);
    }
    
    struct bullet bullets[MAX_BULLETS_ON_SCREEN];
    int bullets_count = 0;

    bamboo_generate(w, h);

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {

        if (IsKeyPressed(KEY_DEBUG_TOGGLE)) {
            debug_enabled = !debug_enabled;
        }
        
        if (IsKeyPressed(KEY_F5)) {
            bamboo_generate(w, h);
        }
        
        w = GetScreenWidth();
        h = GetScreenHeight();

        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) 
            velocity.x -= X_SPEED_INCREMENT;
        
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) 
            velocity.x += X_SPEED_INCREMENT;
        
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        {
            if (player_jump_count < 2)
            {
                velocity.y = -JUMP_SPEED;
                player_jump_count++;
            }
        }

        float speed = Vector2Length(velocity);
        if (speed > SPEED_LIMIT)
        {
            velocity = Vector2Scale(Vector2Normalize(velocity), SPEED_LIMIT);
        }

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
            player_jump_count = 0;
        }

        velocity.x *= FRICTION;
        velocity.y += GRAVITY;

        last_coords[last_coords_count++] = (Vector2){ player.x, player.y };

        if (last_coords_count >= ARRAY_SIZE(last_coords))
            last_coords_count = 0;

        for (int index = 0; index < ARRAY_SIZE(collectables); index += 1)
        {
            if (CheckCollisionCircleRec(collectables[index], COLLECTABLE_RADIUS, player))
            {
                collectables[index].x = GetRandomValue(2 * COLLECTABLE_RADIUS, w - 2 * COLLECTABLE_RADIUS);
                collectables[index].y = GetRandomValue(2 * COLLECTABLE_RADIUS, h - 2 * COLLECTABLE_RADIUS);

                player_score+=combo_mult;
                combo_mult++;
                combo_drop_timer = COMBO_DROP_TIME;
            }
        }

        if (velocity.y > 0) {
            for (int index = 0; index < BAMBOO_CAP; ++index) {
                Vector2 b = bamboo[index];
                float fudge_factor = velocity.y;
                int feet_above_bamboo = player.y + PLAYER_HEIGHT < b.y + fudge_factor;
                int would_fall_below = player.y + PLAYER_HEIGHT + velocity.y >= b.y;
                if (feet_above_bamboo && would_fall_below && CheckCollisionCircleRec(b, bamboo_width * 2.0f, player)) {
                    if (player_jump_count > 0)
                    {
                        bamboo_wobble[index] += 20;
                    }
                    player.y = b.y - PLAYER_HEIGHT;
                    velocity.y = 0;
                    player_jump_count = 0;
                }
            }
        }

        if(combo_drop_timer>0.f){
            combo_drop_timer=fmax(0.f,combo_drop_timer-GetFrameTime());
            if(combo_drop_timer==0.f){
                combo_mult=1;
            }
        }

        Rectangle gun = {
            .x = player.x + player.width / 2 - GUN_WIDTH / 2,
            .y = player.y + player.height / 2,
            .width = GUN_WIDTH,
            .height = GUN_HEIGHT,
        };
        Vector2 gun_direction = Vector2Normalize(Vector2Subtract(GetMousePosition(), (Vector2) { gun.x + GUN_WIDTH / 2, gun.y }));
        if (bullets_count < MAX_BULLETS_ON_SCREEN && IsMouseButtonPressed(0)) {
            bullets[bullets_count].pos = (Vector2) { .x = gun.x + GUN_WIDTH / 2, .y = gun.y };
            bullets[bullets_count].dir = gun_direction;
            bullets_count += 1;
        }
        for (int index = 0; index < bullets_count; index += 1)
        {
            Vector2 pos = Vector2Add(bullets[index].pos, Vector2Scale(bullets[index].dir, BULLET_SPEED * GetFrameTime()));
            if (pos.x < 0 || pos.x > w || pos.y < 0 || pos.y > h)
            {
                memmove(bullets + index, bullets + index + 1, sizeof *bullets * (bullets_count - index - 1));
                index -= 1;
                bullets_count -= 1;
            }
            else
            {
                bullets[index].pos = pos;
            }
        }

        BeginDrawing();

        ClearBackground((Color) { 0, 0, 0, 255 });
        
        Color color_sky = GetColor(0xcbddaaff);
        Color color_base = GetColor(0x47d666ff);
        DrawRectangleGradientV(0, 0, w, h, color_sky, color_base);
        
        for (int i = 0; i < BAMBOO_CAP; i++) {
            bamboo_wobble[i] *= 0.5;
            Vector2 src = bamboo[i];
            Vector2 dst = (Vector2){src.x + bamboo_tilt[i], h};
            src.x += bamboo_wobble[i];
            DrawLineEx(src, dst, BAMBOO_THICC, BAMBOO_COLOR);
        }
        
        for (int i = 0; i < ARRAY_SIZE(last_coords); i++)
        {
            Vector2 coords = last_coords[i];
            float radius = 10 + fabsf(sinf(GetTime())) * 5;

            DrawCircle(coords.x + player.width / 2, coords.y + player.height / 2, radius, (Color){ 0, 121, 241, 255 });
        }
        
        for (int index = 0; index < ARRAY_SIZE(collectables); index += 1)
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

        rlPushMatrix();
        {
            rlTranslatef(gun.x + GUN_WIDTH / 2, gun.y, 0);
            float dot = Vector2DotProduct((Vector2) { 0, 1 }, gun_direction);
            float angle = gun_direction.x > 0 ? acosf(dot) : -acosf(dot);
            rlRotatef(angle * (180.f / PI), 0, 0, -1);
            DrawRectangle(-GUN_WIDTH / 2, 0, gun.width, gun.height, (Color) { 0xFF, 0x33, 0x33, 0xFF });
        }
        rlPopMatrix();

        for (int index = 0; index < bullets_count; index += 1) {
            struct bullet b = bullets[index];
            DrawCircle(b.pos.x, b.pos.y, BULLET_RADIUS, (Color) {0xFF, 0x44, 0x44, 0xFF });
        }

        if (debug_enabled) {
            DrawCircleV((Vector2){player.x, player.y}, 6.0f, MAGENTA);
        }

        /////////////
        //HUD overlay
        /////////////
        
        //Score display
        const char*score_str = TextFormat("Score: %d",player_score);
        Vector2 score_text_size = MeasureTextEx(GetFontDefault(),score_str,SCORE_FONT_SIZE,2);
        DrawTextEx(GetFontDefault(),score_str,(Vector2){GetScreenWidth()-score_text_size.x-SCORE_PADDING,0},SCORE_FONT_SIZE,2,BLACK); //Right-aligned to screen edge

        //Combobar display
        Vector2 combo_bar_upper_left_corner = (Vector2){GetScreenWidth()-COMBO_BAR_PADDING-COMBO_BAR_WIDTH,score_text_size.y+COMBO_BAR_PADDING};
        int remainingtime_combobar_width = combo_drop_timer/COMBO_DROP_TIME*COMBO_BAR_WIDTH;

        Color interpolated_remainingtime_col = ColorLerp(COMBO_BAR_EMPTY_COLOR,COMBO_BAR_FULL_COLOR,combo_drop_timer/COMBO_DROP_TIME);
        DrawRectangle(combo_bar_upper_left_corner.x,combo_bar_upper_left_corner.y,remainingtime_combobar_width,COMBO_BAR_HEIGHT,interpolated_remainingtime_col);
        DrawRectangleLines(combo_bar_upper_left_corner.x,combo_bar_upper_left_corner.y,COMBO_BAR_WIDTH,COMBO_BAR_HEIGHT,BLACK);

        //Combo text display
        const char*combo_str = TextFormat("x%d",combo_mult);
        Vector2 combo_text_size = MeasureTextEx(GetFontDefault(),combo_str,COMBO_FONT_SIZE,0);
        DrawTextEx(GetFontDefault(),combo_str,(Vector2){combo_bar_upper_left_corner.x+COMBO_BAR_WIDTH-combo_text_size.x-COMBO_PADDING,combo_bar_upper_left_corner.y+COMBO_BAR_HEIGHT-combo_text_size.y},COMBO_FONT_SIZE,0,BLACK);

        /////////////

        EndDrawing();
    }

    return 0;
}

const float lerp(const float f1, const float f2, const float t){
    return f1*(1-t)+f2*t;
}

const Color ColorLerp(const Color c1, const Color c2, const float t){
    return (Color){
        lerp(c1.r,c2.r,t),
        lerp(c1.g,c2.g,t),
        lerp(c1.b,c2.b,t),
        lerp(c1.a,c2.a,t),
    };
}

void bamboo_generate(int w, int h) {
    float bamboo_heightline = h - (float)h * 0.4f;
    float bamboo_height_variation = bamboo_heightline * 0.3f;
    float bamboo_x_variation = bamboo_width * 10.0f;
    float bamboo_step = w / ((float)BAMBOO_CAP - 1);
    for (int index = 0; index < BAMBOO_CAP; ++index) {
        bamboo[index].y = bamboo_heightline + GetRandomValue(-bamboo_height_variation, bamboo_height_variation);
        bamboo[index].x = bamboo_step * index + GetRandomValue(-bamboo_x_variation, bamboo_x_variation);
        bamboo_tilt[index] = GetRandomValue(-bamboo_x_variation, bamboo_x_variation);
    }
}
