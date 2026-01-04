/*
** EPITECH PROJECT, 2025
** My_hunter
** File description:
** mario runner helpers
*/

#include "../../include/my.h"

struct runner_skin_config {
    runner_type_t type;
    const char *texture_path;
    sfVector2f scale;
    float frame_time;
    size_t frame_count;
    const sfIntRect *frames;
};

static const sfIntRect MARIO_FRAME_RECTS[MARIO_FRAME_COUNT] = {
    {5, 2, 48, 53},
    {79, 2, 48, 51},
    {151, 2, 48, 53},
    {223, 2, 48, 51},
    {293, 2, 49, 53},
    {6, 75, 49, 52},
    {77, 75, 49, 50},
    {149, 75, 49, 50},
    {221, 75, 49, 52},
    {293, 73, 49, 52}
};

static const sfIntRect GOOMBA_FRAME_RECTS[GOOMBA_FRAME_COUNT] = {
    {20, 13, 81, 108},
    {116, 13, 74, 108},
    {202, 13, 70, 108},
    {287, 13, 64, 108},
    {366, 13, 64, 108},
    {442, 13, 70, 108},
    {524, 13, 74, 108},
    {606, 13, 81, 108}
};

static const sfIntRect KOOPA_FRAME_RECTS[KOOPA_FRAME_COUNT] = {
    {32, 24, 236, 251},
    {285, 24, 236, 251},
    {544, 24, 236, 251},
    {797, 24, 236, 251}
};

static const struct runner_skin_config RUNNER_SKINS[RUNNER_TYPE_COUNT] = {
    {RUNNER_TYPE_MARIO, "tx/gameplay/runners/mario2.png",
        {MARIO_SCALE, MARIO_SCALE}, MARIO_ANIM_FRAME_TIME, MARIO_FRAME_COUNT,
        MARIO_FRAME_RECTS},
    {RUNNER_TYPE_GOOMBA, "tx/gameplay/runners/goomba.png",
        {1.35f, 1.35f}, 0.12f, GOOMBA_FRAME_COUNT, GOOMBA_FRAME_RECTS},
    {RUNNER_TYPE_KOOPA, "tx/gameplay/runners/koopa.png",
        {0.60f, 0.60f}, 0.10f, KOOPA_FRAME_COUNT, KOOPA_FRAME_RECTS}
};

static const struct runner_skin_config *mario_get_skin(runner_type_t type)
{
    for (size_t i = 0; i < RUNNER_TYPE_COUNT; i++) {
        if (RUNNER_SKINS[i].type == type)
            return &RUNNER_SKINS[i];
    }
    return NULL;
}

static int mario_load_extra_textures(mario_t *mario)
{
    sfTexture *texture;

    if (mario == NULL)
        return 84;
    for (size_t i = 0; i < RUNNER_TYPE_COUNT; i++) {
        if (RUNNER_SKINS[i].type == RUNNER_TYPE_MARIO)
            continue;
        texture = sfTexture_createFromFile(RUNNER_SKINS[i].texture_path, NULL);
        if (texture == NULL)
            return 84;
        mario->textures[RUNNER_SKINS[i].type] = texture;
    }
    return 0;
}

static int mario_init_sprite(mario_t *mario)
{
    const struct runner_skin_config *skin = mario_get_skin(RUNNER_TYPE_MARIO);
    const sfVector2f start_pos = {0.0f, 0.0f};

    if (skin == NULL || mario == NULL)
        return 84;
    if (sprite_init(&mario->sprite, skin->texture_path, &start_pos,
            &skin->scale) == 84)
        return 84;
    mario->textures[RUNNER_TYPE_MARIO] = mario->sprite.texture;
    return 0;
}

static void mario_reset_struct(mario_t *mario)
{
    if (mario == NULL)
        return;
    mario->sprite.sprite = NULL;
    mario->sprite.texture = NULL;
    mario->anim_clock = NULL;
    mario->active = sfFalse;
    mario->frame_count = 0;
    mario->current_frame = 0;
    mario->frame_time = MARIO_ANIM_FRAME_TIME;
    mario->hit_score_delta = 0;
    mario->escape_score_delta = 0;
    mario->exit_limit = 0.0f;
    mario->exit_vertical = sfFalse;
    mario->exit_reverse = sfFalse;
    mario->velocity = (sfVector2f){MARIO_BASE_SPEED, 0.0f};
    for (size_t i = 0; i < RUNNER_TYPE_COUNT; i++)
        mario->textures[i] = NULL;
}

int mario_init(mario_t *mario)
{
    if (mario == NULL)
        return 84;
    mario_reset_struct(mario);
    if (mario_init_sprite(mario) == 84
        || mario_load_extra_textures(mario) == 84) {
        mario_destroy(mario);
        return 84;
    }
    mario->anim_clock = sfClock_create();
    if (mario->anim_clock == NULL) {
        mario_destroy(mario);
        return 84;
    }
    mario_set_type(mario, RUNNER_TYPE_MARIO);
    return 0;
}

void mario_destroy(mario_t *mario)
{
    if (mario == NULL)
        return;
    if (mario->anim_clock != NULL)
        sfClock_destroy(mario->anim_clock);
    mario->anim_clock = NULL;
    if (mario->sprite.sprite != NULL)
        sfSprite_destroy(mario->sprite.sprite);
    mario->sprite.sprite = NULL;
    for (size_t i = 0; i < RUNNER_TYPE_COUNT; i++) {
        if (mario->textures[i] != NULL)
            sfTexture_destroy(mario->textures[i]);
        mario->textures[i] = NULL;
    }
    mario->sprite.texture = NULL;
    mario->active = sfFalse;
}

void mario_set_type(mario_t *mario, runner_type_t type)
{
    const struct runner_skin_config *skin = mario_get_skin(type);

    if (mario == NULL || skin == NULL)
        return;
    mario->type = type;
    mario->frame_time = skin->frame_time;
    mario->frame_count = (skin->frame_count > MARIO_FRAME_COUNT)
        ? MARIO_FRAME_COUNT : skin->frame_count;
    mario->current_frame = 0;
    for (size_t i = 0; i < mario->frame_count; i++)
        mario->frames[i] = skin->frames[i];
    if (type < RUNNER_TYPE_COUNT && mario->textures[type] != NULL) {
        mario->sprite.texture = mario->textures[type];
        sfSprite_setTexture(mario->sprite.sprite, mario->textures[type],
            sfTrue);
    }
    sfSprite_setScale(mario->sprite.sprite, skin->scale);
    if (mario->frame_count > 0)
        sprite_set_frame(&mario->sprite, &mario->frames[0]);
}
