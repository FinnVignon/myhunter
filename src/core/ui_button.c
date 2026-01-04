/*
** EPITECH PROJECT, 2025
** My_hunter
** File description:
** button helpers
*/

#include "../../include/my.h"

int button_init(button_t *button, const button_config_t *config)
{
    if (button == NULL || config == NULL)
        return 84;
    if (sprite_init(&button->sprite, config->texture_path,
            &config->position, &config->scale) == 84)
        return 84;
    button->on_click = config->callback;
    button->context = NULL;
    return 0;
}

void button_set_context(button_t *button, void *context)
{
    if (button == NULL)
        return;
    button->context = context;
}

void button_destroy(button_t *button)
{
    if (button == NULL)
        return;
    sprite_destroy(&button->sprite);
    button->on_click = NULL;
    button->context = NULL;
}

void button_draw(sfRenderWindow *window, const button_t *button)
{
    if (window == NULL || button == NULL)
        return;
    sfRenderWindow_drawSprite(window, button->sprite.sprite, NULL);
}

void button_handle_click(const button_t *button, const sfVector2i *mouse)
{
    sfFloatRect bounds;

    if (button == NULL || button->sprite.sprite == NULL || mouse == NULL)
        return;
    bounds = sfSprite_getGlobalBounds(button->sprite.sprite);
    if (!sfFloatRect_contains(&bounds, (float)mouse->x, (float)mouse->y))
        return;
    if (button->on_click != NULL)
        button->on_click(button->context);
}
