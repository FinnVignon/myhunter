/*
** EPITECH PROJECT, 2025
** My_hunter
** File description:
** mario runner pool and timing resources
*/

#include "../../include/my.h"

static void gameplay_destroy_partial_marios(gameplay_t *gameplay,
    size_t count)
{
    if (gameplay == NULL)
        return;
    for (size_t i = 0; i < count; i++)
        mario_destroy(&gameplay->marios[i]);
}

static int gameplay_create_marios(gameplay_t *gameplay)
{
    if (gameplay == NULL)
        return 84;
    for (size_t i = 0; i < MARIO_MAX_COUNT; i++) {
        if (mario_init(&gameplay->marios[i]) == 84) {
            gameplay_destroy_partial_marios(gameplay, i);
            return 84;
        }
    }
    return 0;
}

int gameplay_init_mario_system(gameplay_t *gameplay)
{
    if (gameplay == NULL)
        return 84;
    if (gameplay_create_marios(gameplay) == 84)
        return 84;
    gameplay->mario_spawn_clock = sfClock_create();
    gameplay->enemy_spawn_clock = sfClock_create();
    gameplay->delta_clock = sfClock_create();
    gameplay->round_clock = sfClock_create();
    if (gameplay->mario_spawn_clock == NULL
        || gameplay->enemy_spawn_clock == NULL
        || gameplay->delta_clock == NULL
        || gameplay->round_clock == NULL) {
        gameplay_destroy_mario_system(gameplay);
        return 84;
    }
    sfClock_restart(gameplay->mario_spawn_clock);
    sfClock_restart(gameplay->enemy_spawn_clock);
    sfClock_restart(gameplay->delta_clock);
    sfClock_restart(gameplay->round_clock);
    return 0;
}

void gameplay_destroy_mario_system(gameplay_t *gameplay)
{
    if (gameplay == NULL)
        return;
    for (size_t i = 0; i < MARIO_MAX_COUNT; i++)
        mario_destroy(&gameplay->marios[i]);
    if (gameplay->mario_spawn_clock != NULL)
        sfClock_destroy(gameplay->mario_spawn_clock);
    if (gameplay->enemy_spawn_clock != NULL)
        sfClock_destroy(gameplay->enemy_spawn_clock);
    if (gameplay->delta_clock != NULL)
        sfClock_destroy(gameplay->delta_clock);
    if (gameplay->round_clock != NULL)
        sfClock_destroy(gameplay->round_clock);
    gameplay->mario_spawn_clock = NULL;
    gameplay->enemy_spawn_clock = NULL;
    gameplay->delta_clock = NULL;
    gameplay->round_clock = NULL;
}
