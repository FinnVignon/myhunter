/*
** EPITECH PROJECT, 2025
** My_hunter
** File description:
** mario state helpers
*/

#include "../../include/my.h"

void mario_activate(mario_t *mario, const sfVector2f *pos)
{
    if (mario == NULL || pos == NULL)
        return;
    mario->active = sfTrue;
    mario->current_frame = 0;
    if (mario->frame_count > 0)
        sprite_set_frame(&mario->sprite, &mario->frames[0]);
    sfClock_restart(mario->anim_clock);
    sfSprite_setPosition(mario->sprite.sprite, *pos);
}

void mario_deactivate(mario_t *mario)
{
    if (mario == NULL)
        return;
    mario->active = sfFalse;
    mario->velocity = (sfVector2f){0.0f, 0.0f};
    mario->hit_score_delta = 0;
    mario->escape_score_delta = 0;
    mario->exit_limit = 0.0f;
    mario->exit_vertical = sfFalse;
    mario->exit_reverse = sfFalse;
}

static void mario_step_animation(mario_t *mario)
{
    sfTime elapsed;
    float seconds;

    if (mario == NULL || mario->anim_clock == NULL || mario->frame_count == 0
        || mario->frame_time <= 0.0f)
        return;
    elapsed = sfClock_getElapsedTime(mario->anim_clock);
    seconds = sfTime_asSeconds(elapsed);
    if (seconds < mario->frame_time)
        return;
    mario->current_frame = (mario->current_frame + 1) % mario->frame_count;
    sprite_set_frame(&mario->sprite, &mario->frames[mario->current_frame]);
    sfClock_restart(mario->anim_clock);
}

void mario_update(mario_t *mario, float delta_time)
{
    sfVector2f offset;

    if (mario == NULL || mario->active == sfFalse)
        return;
    offset.x = mario->velocity.x * delta_time;
    offset.y = mario->velocity.y * delta_time;
    sfSprite_move(mario->sprite.sprite, offset);
    mario_step_animation(mario);
}
