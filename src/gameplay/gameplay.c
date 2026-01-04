/*
** EPITECH PROJECT, 2025
** My_hunter
** File description:
** gameplay orchestration
*/

#include "../../include/my.h"

static void gameplay_set_defaults(gameplay_t *gameplay)
{
    gameplay->ui_font = NULL;
    gameplay->score_text = NULL;
    gameplay->timer_text = NULL;
    gameplay->end_text = NULL;
    gameplay->escape_text = NULL;
    gameplay->escape_clock = NULL;
    gameplay->escape_visible = sfTrue;
    gameplay->round_clock = NULL;
    gameplay->score = 0;
    leaderboard_init(&gameplay->leaderboard);
    gameplay->round_over = sfFalse;
    gameplay->timer_expired = sfFalse;
    gameplay->music_round = NULL;
    gameplay->music_end = NULL;
}

static void gameplay_load_leaderboard(gameplay_t *gameplay)
{
    if (gameplay == NULL)
        return;
    leaderboard_load(&gameplay->leaderboard, NULL);
}

static int gameplay_init_components(gameplay_t *gameplay)
{
    if (gameplay_init_background(gameplay) == 84)
        return 84;
    if (gameplay_init_pipes(gameplay) == 84)
        return 84;
    if (gameplay_init_mario_system(gameplay) == 84)
        return 84;
    if (gameplay_init_ui(gameplay) == 84)
        return 84;
    return 0;
}

static int gameplay_init_music(gameplay_t *gameplay)
{
    const char *round_path = "./audio/gameplay.mp3";
    const char *end_path = "./audio/KO.mp3";

    gameplay->music_round = sfMusic_createFromFile(round_path);
    if (gameplay->music_round == NULL)
        return 84;
    gameplay->music_end = sfMusic_createFromFile(end_path);
    if (gameplay->music_end == NULL) {
        sfMusic_destroy(gameplay->music_round);
        gameplay->music_round = NULL;
        return 84;
    }
    sfMusic_setLoop(gameplay->music_round, sfTrue);
    sfMusic_setLoop(gameplay->music_end, sfTrue);
    return 0;
}

int init_gameplay(gameplay_t *gameplay)
{
    if (gameplay == NULL)
        return 84;
    gameplay_set_defaults(gameplay);
    if (gameplay_init_components(gameplay) == 84
        || gameplay_init_music(gameplay) == 84) {
        destroy_gameplay(gameplay);
        return 84;
    }
    gameplay_load_leaderboard(gameplay);
    gameplay_reset_round(gameplay);
    return 0;
}

void destroy_gameplay(gameplay_t *gameplay)
{
    if (gameplay == NULL)
        return;
    gameplay_stop_music(gameplay);
    if (gameplay->music_round != NULL)
        sfMusic_destroy(gameplay->music_round);
    if (gameplay->music_end != NULL)
        sfMusic_destroy(gameplay->music_end);
    gameplay->music_round = NULL;
    gameplay->music_end = NULL;
    gameplay_destroy_ui(gameplay);
    gameplay_destroy_mario_system(gameplay);
    gameplay_destroy_pipes(gameplay);
    gameplay_destroy_background(gameplay);
}

void update_gameplay(gameplay_t *gameplay)
{
    float delta_time;

    if (gameplay == NULL)
        return;
    gameplay_ui_tick(gameplay);
    if (gameplay->round_over == sfTrue)
        return;
    delta_time = gameplay_consume_delta(gameplay);
    gameplay_spawn_runner(gameplay);
    gameplay_update_runners(gameplay, delta_time);
}

void draw_gameplay(sfRenderWindow *window, const gameplay_t *gameplay)
{
    if (window == NULL || gameplay == NULL)
        return;
    sfRenderWindow_drawSprite(window, gameplay->background.sprite, NULL);
    for (size_t i = 0; i < MARIO_MAX_COUNT; i++)
        mario_draw(window, &gameplay->marios[i]);
    gameplay_draw_pipes(window, gameplay);
    gameplay_draw_ui(window, gameplay);
}

void gameplay_handle_event(gameplay_t *gameplay, state_t *state,
    const sfEvent *event)
{
    sfVector2i mouse = {0, 0};

    if (gameplay == NULL || state == NULL || event == NULL)
        return;
    if (event->type == sfEvtKeyPressed && event->key.code == sfKeyEscape)
        *state = STATE_MENU;
    if (event->type != sfEvtMouseButtonPressed
        || event->mouseButton.button != sfMouseLeft)
        return;
    if (gameplay->round_over == sfTrue)
        return;
    mouse.x = event->mouseButton.x;
    mouse.y = event->mouseButton.y;
    gameplay_handle_runner_click(gameplay, &mouse);
}
