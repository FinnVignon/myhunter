/*
** EPITECH PROJECT, 2025
** My_hunter
** File description:
** custom target cursor helpers
*/

#include "../../include/my.h"

int target_init(target_t *target)
{
    static const sfVector2f TARGET_SCALE = {0.18f, 0.18f};
    sfVector2f center;
    sfVector2u size;
    int status;

    if (target == NULL)
        return 84;
    status = sprite_init(&target->sprite, "tx/gameplay/target.png", NULL,
        &TARGET_SCALE);
    if (status == 84)
        return 84;
    size = sfTexture_getSize(target->sprite.texture);
    center.x = (float)size.x / 2.0f;
    center.y = (float)size.y / 2.0f;
    sfSprite_setOrigin(target->sprite.sprite, center);
    return 0;
}

void target_destroy(target_t *target)
{
    if (target == NULL)
        return;
    sprite_destroy(&target->sprite);
}

void target_update(target_t *target, sfRenderWindow *window)
{
    sfVector2i mouse;
    sfVector2f pos;

    if (target == NULL || window == NULL || target->sprite.sprite == NULL)
        return;
    mouse = sfMouse_getPositionRenderWindow(window);
    pos.x = (float)mouse.x;
    pos.y = (float)mouse.y;
    sfSprite_setPosition(target->sprite.sprite, pos);
}

void target_draw(sfRenderWindow *window, const target_t *target)
{
    if (window == NULL || target == NULL || target->sprite.sprite == NULL)
        return;
    sfRenderWindow_drawSprite(window, target->sprite.sprite, NULL);
}
