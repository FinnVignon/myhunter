/*
** EPITECH PROJECT, 2025
** My_hunter
** File description:
** tutorial screen
*/

#include "../../include/my.h"

int init_tutorial(tutorial_t *tutorial)
{
    const sfVector2f pos = {0.0f, 0.0f};
    const sfVector2f scale = {1.0f, 1.0f};
    const sfVector2u size = {WINDOW_WIDTH, WINDOW_HEIGHT};
    const char *music_path = "./audio/tutorial.mp3";

    tutorial->music = NULL;
    if (sprite_init(&tutorial->screen, "tx/screens/tutorial.png", &pos,
            &scale) == 84)
        return 84;
    sprite_fit_screen(&tutorial->screen, &size);
    tutorial->music = sfMusic_createFromFile(music_path);
    if (tutorial->music == NULL) {
        sprite_destroy(&tutorial->screen);
        return 84;
    }
    sfMusic_setLoop(tutorial->music, sfTrue);
    return 0;
}

void destroy_tutorial(tutorial_t *tutorial)
{
    sprite_destroy(&tutorial->screen);
    if (tutorial->music != NULL)
        sfMusic_destroy(tutorial->music);
    tutorial->music = NULL;
}

void draw_tutorial(sfRenderWindow *window, const tutorial_t *tutorial)
{
    sfRenderWindow_drawSprite(window, tutorial->screen.sprite, NULL);
}

void tutorial_handle_event(state_t *state)
{
    if (sfKeyboard_isKeyPressed(sfKeySpace))
        *state = STATE_GAME;
}
