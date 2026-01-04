/*
** EPITECH PROJECT, 2025
** My_hunter
** File description:
** escape hint text helpers
*/

#include "../../include/my.h"

static const sfVector2f ESCAPE_TEXT_POSITION = {WINDOW_WIDTH / 2.0f,
    WINDOW_HEIGHT - 80.0f};
static const char ESCAPE_TEXT_LABEL[] =
    "Press the Escape button to go back to the menu";

void gameplay_prepare_escape_hint(gameplay_t *gameplay)
{
    sfFloatRect bounds;
    sfVector2f origin;

    if (gameplay == NULL || gameplay->escape_text == NULL)
        return;
    sfText_setString(gameplay->escape_text, ESCAPE_TEXT_LABEL);
    bounds = sfText_getLocalBounds(gameplay->escape_text);
    origin.x = bounds.left + bounds.width / 2.0f;
    origin.y = bounds.top + bounds.height / 2.0f;
    sfText_setOrigin(gameplay->escape_text, origin);
    sfText_setPosition(gameplay->escape_text, ESCAPE_TEXT_POSITION);
}
