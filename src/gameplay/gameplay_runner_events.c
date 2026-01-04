/*
** EPITECH PROJECT, 2025
** My_hunter
** File description:
** mario runner update and input helpers
*/

#include "../../include/my.h"

static sfBool runner_should_exit(const mario_t *runner,
    const sfVector2f *position)
{
    float value;

    if (runner == NULL || position == NULL || runner->active == sfFalse)
        return sfFalse;
    value = (runner->exit_vertical == sfTrue) ? position->y : position->x;
    if (runner->exit_reverse == sfTrue)
        return value <= runner->exit_limit;
    return value >= runner->exit_limit;
}

static int runner_exit_score_delta(const mario_t *runner)
{
    if (runner == NULL)
        return 0;
    if (runner->type != RUNNER_TYPE_MARIO)
        return 0;
    if (runner->exit_vertical == sfTrue || runner->exit_reverse == sfTrue)
        return 0;
    return -1;
}

void gameplay_update_runners(gameplay_t *gameplay, float delta_time)
{
    sfVector2f position;

    if (gameplay == NULL || gameplay->round_over == sfTrue)
        return;
    for (size_t i = 0; i < MARIO_MAX_COUNT; i++) {
        mario_update(&gameplay->marios[i], delta_time);
        if (gameplay->marios[i].active == sfFalse)
            continue;
        position = sfSprite_getPosition(gameplay->marios[i].sprite.sprite);
        if (runner_should_exit(&gameplay->marios[i], &position) == sfFalse)
            continue;
        mario_deactivate(&gameplay->marios[i]);
        gameplay_add_score(gameplay,
            runner_exit_score_delta(&gameplay->marios[i]));
    }
}

void gameplay_handle_runner_click(gameplay_t *gameplay,
    const sfVector2i *mouse)
{
    if (gameplay == NULL || mouse == NULL)
        return;
    for (size_t i = 0; i < MARIO_MAX_COUNT; i++) {
        if (mario_contains_point(&gameplay->marios[i], mouse) == sfFalse)
            continue;
        gameplay_add_score(gameplay, gameplay->marios[i].hit_score_delta);
        mario_deactivate(&gameplay->marios[i]);
        return;
    }
}
