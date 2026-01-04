/*
** EPITECH PROJECT, 2025
** My_hunter
** File description:
** main loop
*/

#include "../../include/my.h"
#include <time.h>

static void game_apply_state_music(game_t *game)
{
    if (game == NULL)
        return;
    if (game->menu.music != NULL)
        sfMusic_stop(game->menu.music);
    if (game->tutorial.music != NULL)
        sfMusic_stop(game->tutorial.music);
    gameplay_stop_music(&game->gameplay);
    if (game->state == STATE_MENU && game->menu.music != NULL)
        sfMusic_play(game->menu.music);
    if (game->state == STATE_TUTORIAL && game->tutorial.music != NULL)
        sfMusic_play(game->tutorial.music);
    if (game->state != STATE_GAME)
        return;
    if (game->gameplay.round_over == sfTrue) {
        gameplay_play_end_music(&game->gameplay);
        return;
    }
    gameplay_start_round_music(&game->gameplay);
}

static int init_game_assets(game_t *game)
{
    if (init_menu(&game->menu) == 84)
        return 84;
    if (init_tutorial(&game->tutorial) == 84)
        return 84;
    if (init_gameplay(&game->gameplay) == 84)
        return 84;
    if (target_init(&game->target) == 84)
        return 84;
    sfSprite_setPosition(game->target.sprite.sprite, (sfVector2f){
            WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f});
    return 0;
}

static int init_game(game_t *game)
{
    sfVideoMode mode = {WINDOW_WIDTH, WINDOW_HEIGHT, 32};

    game->window = sfRenderWindow_create(mode, "Mario Hunter",
        sfResize | sfClose, NULL);
    if (game->window == NULL)
        return 84;
    if (init_game_assets(game) == 84)
        return 84;
    game->state = STATE_MENU;
    sfRenderWindow_setFramerateLimit(game->window, 60);
    game_apply_state_music(game);
    sfRenderWindow_setMouseCursorVisible(game->window, sfTrue);
    return 0;
}

static void destroy_game(game_t *game)
{
    destroy_menu(&game->menu);
    destroy_tutorial(&game->tutorial);
    destroy_gameplay(&game->gameplay);
    target_destroy(&game->target);
    if (game->window != NULL)
        sfRenderWindow_destroy(game->window);
}

static void dispatch_event(game_t *game, const sfEvent *event)
{
    if (game == NULL || event == NULL)
        return;
    if (game->state == STATE_MENU)
        menu_handle_event(game->window, &game->menu, &game->state, event);
    if (game->state == STATE_TUTORIAL)
        tutorial_handle_event(&game->state);
    if (game->state == STATE_GAME)
        gameplay_handle_event(&game->gameplay, &game->state, event);
}

static void handle_state_transition(game_t *game, state_t previous_state)
{
    if (game == NULL || previous_state == game->state)
        return;
    if (game->state == STATE_GAME)
        gameplay_reset_round(&game->gameplay);
    game_apply_state_music(game);
    if (game->window != NULL)
        sfRenderWindow_setMouseCursorVisible(game->window,
            (game->state == STATE_GAME) ? sfFalse : sfTrue);
}

static void handle_events(game_t *game)
{
    sfEvent event;
    state_t previous_state = game->state;

    while (sfRenderWindow_pollEvent(game->window, &event)) {
        if (event.type == sfEvtClosed)
            sfRenderWindow_close(game->window);
        dispatch_event(game, &event);
    }
    handle_state_transition(game, previous_state);
}

static void render(game_t *game)
{
    sfRenderWindow_clear(game->window, sfBlack);
    if (game->state == STATE_MENU)
        draw_menu(game->window, &game->menu);
    if (game->state == STATE_TUTORIAL)
        draw_tutorial(game->window, &game->tutorial);
    if (game->state == STATE_GAME) {
        draw_gameplay(game->window, &game->gameplay);
        target_draw(game->window, &game->target);
    }
    sfRenderWindow_display(game->window);
}

static void game_step(game_t *game)
{
    if (game == NULL || game->window == NULL)
        return;
    handle_events(game);
    switch (game->state) {
        case STATE_GAME:
            target_update(&game->target, game->window);
            update_gameplay(&game->gameplay);
            break;
        case STATE_MENU:
            update_menu(&game->menu);
            break;
        default:
            break;
    }
    render(game);
}

int main(int argc, char **argv)
{
    game_t game = {0};
    int arg_status = handle_cli_arguments(argc, argv);

    if (arg_status != 0)
        return (arg_status == 1) ? 0 : 84;
    srand((unsigned int)time(NULL));
    if (init_game(&game) == 84) {
        destroy_game(&game);
        return 84;
    }
    while (sfRenderWindow_isOpen(game.window))
        game_step(&game);
    destroy_game(&game);
    return 0;
}
