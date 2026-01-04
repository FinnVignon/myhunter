/*
** EPITECH PROJECT, 2025
** My_hunter
** File description:
** text builder helpers
*/

#include "../../include/text_builder.h"

static int builder_can_write(const text_builder_t *builder)
{
    if (builder == NULL || builder->data == NULL || builder->size == 0)
        return 0;
    if (builder->index + 1 >= builder->size)
        return 0;
    return 1;
}

void text_builder_init(text_builder_t *builder, char *buffer, size_t size)
{
    if (builder == NULL)
        return;
    builder->data = buffer;
    builder->size = size;
    builder->index = 0;
    if (buffer != NULL && size > 0)
        buffer[0] = '\0';
}

void text_builder_push_char(text_builder_t *builder, char character)
{
    if (builder_can_write(builder) == 0)
        return;
    builder->data[builder->index] = character;
    builder->index += 1;
    builder->data[builder->index] = '\0';
}

void text_builder_push_literal(text_builder_t *builder, const char *literal)
{
    size_t i = 0;

    if (builder == NULL || literal == NULL)
        return;
    while (literal[i] != '\0') {
        text_builder_push_char(builder, literal[i]);
        i += 1;
    }
}

static void text_builder_push_unsigned(text_builder_t *builder,
    unsigned long number)
{
    if (number >= 10)
        text_builder_push_unsigned(builder, number / 10);
    text_builder_push_char(builder, (char)('0' + (number % 10)));
}

void text_builder_push_int(text_builder_t *builder, int value)
{
    long number = value;

    if (builder == NULL)
        return;
    if (number == 0) {
        text_builder_push_char(builder, '0');
        return;
    }
    if (number < 0) {
        text_builder_push_char(builder, '-');
        number = -number;
    }
    text_builder_push_unsigned(builder, (unsigned long)number);
}

void text_builder_push_two_digits(text_builder_t *builder, int value)
{
    int clamped = value;

    if (builder == NULL)
        return;
    if (clamped < 0)
        clamped = 0;
    if (clamped > 99)
        clamped = 99;
    text_builder_push_char(builder, (char)('0' + clamped / 10));
    text_builder_push_char(builder, (char)('0' + clamped % 10));
}
