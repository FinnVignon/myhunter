/*
** EPITECH PROJECT, 2025
** My_hunter
** File description:
** gameplay hud rendering helpers
*/

#include "../../include/my.h"

static const char UI_FONT_PATH[] = "tx/fonts/DejaVuSans-Bold.ttf";

struct gameplay_text_config_s {
    unsigned int size;
    sfColor color;
    float outline;
};

static const struct gameplay_text_config_s SCORE_TEXT_CONFIG =
{48, {255, 255, 255, 255}, 2.0f};
static const struct gameplay_text_config_s TIMER_TEXT_CONFIG =
{48, {255, 255, 255, 255}, 2.0f};
static const struct gameplay_text_config_s END_TEXT_CONFIG =
{92, {255, 214, 84, 255}, 4.0f};
static const struct gameplay_text_config_s ESCAPE_TEXT_CONFIG =
{32, {255, 255, 255, 255}, 1.5f};

static void gameplay_setup_text(sfText *text, const sfFont *font,
    const struct gameplay_text_config_s *config)
{
    if (text == NULL || font == NULL || config == NULL)
        return;
    sfText_setFont(text, font);
    sfText_setCharacterSize(text, config->size);
    sfText_setFillColor(text, config->color);
    sfText_setOutlineThickness(text, config->outline);
    sfText_setOutlineColor(text, sfBlack);
}

static int gameplay_create_texts(gameplay_t *gameplay)
{
    gameplay->score_text = sfText_create();
    gameplay->timer_text = sfText_create();
    gameplay->end_text = sfText_create();
    gameplay->escape_text = sfText_create();
    if (gameplay->score_text == NULL || gameplay->timer_text == NULL
        || gameplay->end_text == NULL || gameplay->escape_text == NULL)
        return 84;
    gameplay_setup_text(gameplay->score_text, gameplay->ui_font,
        &SCORE_TEXT_CONFIG);
    gameplay_setup_text(gameplay->timer_text, gameplay->ui_font,
        &TIMER_TEXT_CONFIG);
    gameplay_setup_text(gameplay->end_text, gameplay->ui_font,
        &END_TEXT_CONFIG);
    gameplay_setup_text(gameplay->escape_text, gameplay->ui_font,
        &ESCAPE_TEXT_CONFIG);
    gameplay->escape_clock = sfClock_create();
    if (gameplay->escape_clock == NULL)
        return 84;
    gameplay->escape_visible = sfTrue;
    return 0;
}

int gameplay_init_ui(gameplay_t *gameplay)
{
    if (gameplay == NULL)
        return 84;
    gameplay->ui_font = sfFont_createFromFile(UI_FONT_PATH);
    if (gameplay->ui_font == NULL)
        return 84;
    if (gameplay_create_texts(gameplay) == 84) {
        gameplay_destroy_ui(gameplay);
        return 84;
    }
    return 0;
}

void gameplay_destroy_ui(gameplay_t *gameplay)
{
    if (gameplay == NULL)
        return;
    if (gameplay->score_text != NULL)
        sfText_destroy(gameplay->score_text);
    if (gameplay->timer_text != NULL)
        sfText_destroy(gameplay->timer_text);
    if (gameplay->end_text != NULL)
        sfText_destroy(gameplay->end_text);
    if (gameplay->escape_text != NULL)
        sfText_destroy(gameplay->escape_text);
    if (gameplay->ui_font != NULL)
        sfFont_destroy(gameplay->ui_font);
    if (gameplay->escape_clock != NULL)
        sfClock_destroy(gameplay->escape_clock);
    gameplay->score_text = NULL;
    gameplay->timer_text = NULL;
    gameplay->end_text = NULL;
    gameplay->escape_text = NULL;
    gameplay->ui_font = NULL;
    gameplay->escape_clock = NULL;
}

void gameplay_draw_ui(sfRenderWindow *window, const gameplay_t *gameplay)
{
    if (window == NULL || gameplay == NULL)
        return;
    if (gameplay->score_text != NULL)
        sfRenderWindow_drawText(window, gameplay->score_text, NULL);
    if (gameplay->timer_text != NULL)
        sfRenderWindow_drawText(window, gameplay->timer_text, NULL);
    if (gameplay->escape_text != NULL && gameplay->escape_visible == sfTrue)
        sfRenderWindow_drawText(window, gameplay->escape_text, NULL);
    if (gameplay->round_over == sfTrue && gameplay->end_text != NULL)
        sfRenderWindow_drawText(window, gameplay->end_text, NULL);
}
