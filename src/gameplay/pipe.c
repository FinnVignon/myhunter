/*
** EPITECH PROJECT, 2025
** My_hunter
** File description:
** pipe helpers
*/

#include "../../include/my.h"

int pipe_init(pipe_t *pipe, const pipe_config_t *config)
{
    sfIntRect frame = {6, 490, 30, 35};
    sfVector2f origin = {15.0f, 17.5f};
    const char *texture_path = "tx/gameplay/pipes/mario.png";

    if (pipe == NULL || config == NULL)
        return 84;
    if (sprite_init(&pipe->sprite, texture_path, &config->position,
            &config->scale) == 84)
        return 84;
    sprite_set_frame(&pipe->sprite, &frame);
    sfSprite_setOrigin(pipe->sprite.sprite, origin);
    sfSprite_setRotation(pipe->sprite.sprite, config->rotation);
    return 0;
}

void pipe_destroy(pipe_t *pipe)
{
    if (pipe == NULL)
        return;
    sprite_destroy(&pipe->sprite);
}

void pipe_draw(sfRenderWindow *window, const pipe_t *pipe)
{
    if (window == NULL || pipe == NULL)
        return;
    sfRenderWindow_drawSprite(window, pipe->sprite.sprite, NULL);
}
