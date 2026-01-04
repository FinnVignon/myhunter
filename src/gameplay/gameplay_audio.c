/*
** EPITECH PROJECT, 2025
** My_hunter
** File description:
** gameplay music helpers
*/

#include "../../include/my.h"

void gameplay_start_round_music(gameplay_t *gameplay)
{
    if (gameplay == NULL)
        return;
    if (gameplay->music_end != NULL)
        sfMusic_stop(gameplay->music_end);
    if (gameplay->music_round != NULL)
        sfMusic_play(gameplay->music_round);
}

void gameplay_play_end_music(gameplay_t *gameplay)
{
    if (gameplay == NULL)
        return;
    if (gameplay->music_round != NULL)
        sfMusic_stop(gameplay->music_round);
    if (gameplay->music_end != NULL)
        sfMusic_play(gameplay->music_end);
}

void gameplay_stop_music(gameplay_t *gameplay)
{
    if (gameplay == NULL)
        return;
    if (gameplay->music_round != NULL)
        sfMusic_stop(gameplay->music_round);
    if (gameplay->music_end != NULL)
        sfMusic_stop(gameplay->music_end);
}
