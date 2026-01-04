/*
** EPITECH PROJECT, 2025
** My_hunter
** File description:
** mario drawing helpers
*/

#include "../../include/my.h"

void mario_draw(sfRenderWindow *window, const mario_t *mario)
{
    if (window == NULL || mario == NULL || mario->active == sfFalse)
        return;
    sfRenderWindow_drawSprite(window, mario->sprite.sprite, NULL);
}

sfBool mario_contains_point(const mario_t *mario, const sfVector2i *mouse)
{
    sfFloatRect bounds;

    if (mario == NULL || mouse == NULL || mario->active == sfFalse)
        return sfFalse;
    bounds = sfSprite_getGlobalBounds(mario->sprite.sprite);
    return sfFloatRect_contains(&bounds, (float)mouse->x, (float)mouse->y);
}
