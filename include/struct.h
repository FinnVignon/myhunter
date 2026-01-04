/*
** EPITECH PROJECT, 2025
** My_Hunter
** File description:
** structure header file
*/

#ifndef STRUCT_H_
    #define STRUCT_H_

    #include <stddef.h>

    #include <SFML/Audio.h>
    #include <SFML/Graphics.h>
    #include <SFML/System.h>
    #include <SFML/Window.h>

    #define PIPE_MAX_COUNT 14
    #define MARIO_MAX_COUNT 6
    #define PIPE_SCALE_VALUE 4.0f
    #define RUNNER_TYPE_COUNT 3
    #define PIPE_BASE_WIDTH 30.0f
    #define PIPE_BASE_HEIGHT 35.0f
    #define MARIO_FRAME_COUNT 10
    #define GOOMBA_FRAME_COUNT 8
    #define KOOPA_FRAME_COUNT 4
    #define MARIO_ANIM_FRAME_TIME 0.08f
    #define MARIO_BASE_SPEED 180.0f
    #define MARIO_MIN_SPEED 120.0f
    #define MARIO_MAX_SPEED 520.0f
    #define MARIO_SPEED_PER_POINT 12.0f
    #define MARIO_SPAWN_INTERVAL 1.2f
    #define MARIO_MIN_SPAWN_INTERVAL 0.45f
    #define MARIO_SPAWN_DELTA_PER_POINT 0.015f
    #define ENEMY_SPAWN_INTERVAL 0.9f
    #define ENEMY_MIN_SPAWN_INTERVAL 0.35f
    #define ENEMY_SPAWN_DELTA_PER_POINT 0.01f
    #define MARIO_SCALE 2.0f
    #define MENU_TITLE_FRAME_COUNT 25
    #define GAME_ROUND_DURATION 60.0f
    #define LEADERBOARD_ENTRY_COUNT 3

typedef enum state_e {
    STATE_MENU,
    STATE_TUTORIAL,
    STATE_LAUNCH,
    STATE_GAME
} state_t;

typedef enum runner_type_e {
    RUNNER_TYPE_MARIO,
    RUNNER_TYPE_GOOMBA,
    RUNNER_TYPE_KOOPA
} runner_type_t;

typedef struct sprite_s {
    sfTexture *texture;
    sfSprite *sprite;
} sprite_t;

typedef void (*button_callback_t)(void *context);

typedef struct button_s {
    sprite_t sprite;
    button_callback_t on_click;
    void *context;
} button_t;

typedef struct button_config_s {
    const char *texture_path;
    sfVector2f position;
    sfVector2f scale;
    button_callback_t callback;
} button_config_t;

typedef struct menu_s {
    sprite_t background;
    button_t start_button;
    button_t exit_button;
    sprite_t title;
    sfTexture *title_frame_textures[MENU_TITLE_FRAME_COUNT];
    sfIntRect title_frames[MENU_TITLE_FRAME_COUNT];
    size_t title_frame_count;
    size_t title_current_frame;
    sfClock *title_clock;
    sfMusic *music;
} menu_t;

typedef struct tutorial_s {
    sprite_t screen;
    sfMusic *music;
} tutorial_t;

typedef struct pipe_s {
    sprite_t sprite;
} pipe_t;

typedef struct pipe_config_s {
    sfVector2f position;
    sfVector2f scale;
    float rotation;
} pipe_config_t;

typedef struct mario_s {
    sprite_t sprite;
    sfIntRect frames[MARIO_FRAME_COUNT];
    size_t frame_count;
    size_t current_frame;
    sfClock *anim_clock;
    sfVector2f velocity;
    sfBool active;
    runner_type_t type;
    float frame_time;
    int hit_score_delta;
    int escape_score_delta;
    float exit_limit;
    sfBool exit_vertical;
    sfBool exit_reverse;
    sfTexture *textures[RUNNER_TYPE_COUNT];
} mario_t;

typedef struct leaderboard_s {
    int scores[LEADERBOARD_ENTRY_COUNT];
} leaderboard_t;

typedef struct gameplay_s {
    sprite_t background;
    pipe_t pipes[PIPE_MAX_COUNT];
    size_t pipe_count;
    mario_t marios[MARIO_MAX_COUNT];
    sfClock *mario_spawn_clock;
    sfClock *enemy_spawn_clock;
    sfClock *delta_clock;
    sfClock *round_clock;
    size_t next_spawn_entry;
    sfFont *ui_font;
    sfText *score_text;
    sfText *timer_text;
    sfText *end_text;
    sfText *escape_text;
    sfClock *escape_clock;
    sfBool escape_visible;
    int score;
    leaderboard_t leaderboard;
    sfBool round_over;
    sfBool timer_expired;
    sfMusic *music_round;
    sfMusic *music_end;
} gameplay_t;

typedef struct target_s {
    sprite_t sprite;
} target_t;

typedef struct game_s {
    sfRenderWindow *window;
    state_t state;
    menu_t menu;
    tutorial_t tutorial;
    gameplay_t gameplay;
    target_t target;
} game_t;

#endif /* !STRUCT_H_ */
