/*
** EPITECH PROJECT, 2025
** My_hunter
** File description:
** gameplay timing helper
*/

#include "../../include/my.h"

float gameplay_consume_delta(gameplay_t *gameplay)
{
    sfTime elapsed;

    if (gameplay == NULL || gameplay->delta_clock == NULL)
        return 0.0f;
    elapsed = sfClock_restart(gameplay->delta_clock);
    return sfTime_asSeconds(elapsed);
}
