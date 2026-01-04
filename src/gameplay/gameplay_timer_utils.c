/*
** EPITECH PROJECT, 2025
** My_hunter
** File description:
** gameplay timer text helpers
*/

#include "../../include/my.h"
#include "../../include/text_builder.h"

void gameplay_format_timer_label(char *buffer, size_t size,
    float remaining_time)
{
    text_builder_t builder;
    int minutes;
    int seconds;

    if (buffer == NULL || size == 0)
        return;
    if (remaining_time < 0.0f)
        remaining_time = 0.0f;
    minutes = (int)(remaining_time / 60.0f);
    seconds = (int)remaining_time % 60;
    text_builder_init(&builder, buffer, size);
    text_builder_push_two_digits(&builder, minutes);
    text_builder_push_literal(&builder, ":");
    text_builder_push_two_digits(&builder, seconds);
}
