/*
** EPITECH PROJECT, 2025
** My_hunter
** File description:
** text buffer builder helpers
*/

#ifndef TEXT_BUILDER_H_
    #define TEXT_BUILDER_H_

    #include <stddef.h>

typedef struct text_builder_s {
    char *data;
    size_t size;
    size_t index;
} text_builder_t;

void text_builder_init(text_builder_t *builder, char *buffer, size_t size);
void text_builder_push_char(text_builder_t *builder, char character);
void text_builder_push_literal(text_builder_t *builder, const char *literal);
void text_builder_push_int(text_builder_t *builder, int value);
void text_builder_push_two_digits(text_builder_t *builder, int value);

#endif /* !TEXT_BUILDER_H_ */
