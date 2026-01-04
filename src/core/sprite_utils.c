/*
** EPITECH PROJECT, 2025
** My_hunter
** File description:
** shared sprite helpers
*/

#include "../../include/my.h"

int sprite_init(sprite_t *sprite, const char *path, const sfVector2f *pos,
    const sfVector2f *scale)
{
    if (sprite == NULL)
        return 84;
    sprite->texture = sfTexture_createFromFile(path, NULL);
    if (sprite->texture == NULL)
        return 84;
    sprite->sprite = sfSprite_create();
    if (sprite->sprite == NULL) {
        sprite_destroy(sprite);
        return 84;
    }
    sfSprite_setTexture(sprite->sprite, sprite->texture, sfTrue);
    if (pos != NULL)
        sfSprite_setPosition(sprite->sprite, *pos);
    if (scale != NULL)
        sfSprite_setScale(sprite->sprite, *scale);
    return 0;
}

void sprite_destroy(sprite_t *sprite)
{
    if (sprite == NULL)
        return;
    if (sprite->sprite != NULL)
        sfSprite_destroy(sprite->sprite);
    if (sprite->texture != NULL)
        sfTexture_destroy(sprite->texture);
    sprite->sprite = NULL;
    sprite->texture = NULL;
}

void sprite_fit_screen(sprite_t *sprite, const sfVector2u *target_size)
{
    sfVector2u size;
    sfVector2f scale = {1.0f, 1.0f};

    if (sprite == NULL || sprite->texture == NULL || target_size == NULL)
        return;
    size = sfTexture_getSize(sprite->texture);
    if (size.x != 0 && size.y != 0) {
        scale.x = (float)target_size->x / (float)size.x;
        scale.y = (float)target_size->y / (float)size.y;
    }
    sfSprite_setScale(sprite->sprite, scale);
    sfSprite_setPosition(sprite->sprite, (sfVector2f){0.0f, 0.0f});
}

void sprite_set_frame(sprite_t *sprite, const sfIntRect *rect)
{
    if (sprite == NULL || sprite->sprite == NULL || rect == NULL)
        return;
    sfSprite_setTextureRect(sprite->sprite, *rect);
}
