/*
** EPITECH PROJECT, 2025
** My_hunter
** File description:
** gameplay background and pipe helpers
*/

#include "../../include/my.h"

static const sfVector2f PIPE_SCALE_VECTOR = {PIPE_SCALE_VALUE,
    PIPE_SCALE_VALUE};
static const float PIPE_HALF_HEIGHT = (PIPE_BASE_HEIGHT * PIPE_SCALE_VALUE)
    / 2.0f;
static const float PIPE_BOTTOM_Y = WINDOW_HEIGHT - PIPE_HALF_HEIGHT + 70;
static const float PIPE_TOP_Y = PIPE_HALF_HEIGHT - 60;
static const float PIPE_SIDE_HALF = (PIPE_BASE_HEIGHT * PIPE_SCALE_VALUE - 70)
    / 2.0f;
static const float PIPE_LEFT_X = PIPE_SIDE_HALF;
static const float PIPE_RIGHT_X = WINDOW_WIDTH - PIPE_SIDE_HALF - 60;

static const pipe_config_t PIPE_CONFIGS[] = {
    {{460.0f, PIPE_BOTTOM_Y}, PIPE_SCALE_VECTOR, 0.0f},
    {{760.0f, PIPE_BOTTOM_Y}, PIPE_SCALE_VECTOR, 0.0f},
    {{1060.0f, PIPE_BOTTOM_Y}, PIPE_SCALE_VECTOR, 0.0f},
    {{1360.0f, PIPE_BOTTOM_Y}, PIPE_SCALE_VECTOR, 0.0f},
    {{460.0f, PIPE_TOP_Y}, PIPE_SCALE_VECTOR, 180.0f},
    {{760.0f, PIPE_TOP_Y}, PIPE_SCALE_VECTOR, 180.0f},
    {{1060.0f, PIPE_TOP_Y}, PIPE_SCALE_VECTOR, 180.0f},
    {{1360.0f, PIPE_TOP_Y}, PIPE_SCALE_VECTOR, 180.0f},
    {{PIPE_LEFT_X, 320.0f}, PIPE_SCALE_VECTOR, 90.0f},
    {{PIPE_LEFT_X, 540.0f}, PIPE_SCALE_VECTOR, 90.0f},
    {{PIPE_LEFT_X, 760.0f}, PIPE_SCALE_VECTOR, 90.0f},
    {{PIPE_RIGHT_X, 320.0f}, PIPE_SCALE_VECTOR, 270.0f},
    {{PIPE_RIGHT_X, 540.0f}, PIPE_SCALE_VECTOR, 270.0f},
    {{PIPE_RIGHT_X, 760.0f}, PIPE_SCALE_VECTOR, 270.0f}
};

static const size_t PIPE_CONFIG_COUNT =
    sizeof(PIPE_CONFIGS) / sizeof(PIPE_CONFIGS[0]);

int gameplay_init_background(gameplay_t *gameplay)
{
    const sfVector2f pos = {0.0f, 0.0f};
    const sfVector2f scale = {1.0f, 1.0f};
    const sfVector2u target = {WINDOW_WIDTH, WINDOW_HEIGHT};

    if (sprite_init(&gameplay->background, "tx/gameplay/gamebk.png", &pos,
            &scale) == 84)
        return 84;
    sprite_fit_screen(&gameplay->background, &target);
    return 0;
}

void gameplay_destroy_background(gameplay_t *gameplay)
{
    if (gameplay == NULL)
        return;
    sprite_destroy(&gameplay->background);
}

static int create_pipe(gameplay_t *gameplay, const pipe_config_t *config)
{
    if (config == NULL)
        return 84;
    if (pipe_init(&gameplay->pipes[gameplay->pipe_count], config) == 84)
        return 84;
    gameplay->pipe_count++;
    return 0;
}

int gameplay_init_pipes(gameplay_t *gameplay)
{
    gameplay->pipe_count = 0;
    for (size_t i = 0; i < PIPE_CONFIG_COUNT && i < PIPE_MAX_COUNT; i++) {
        if (create_pipe(gameplay, &PIPE_CONFIGS[i]) == 84) {
            gameplay_destroy_pipes(gameplay);
            return 84;
        }
    }
    return 0;
}

void gameplay_destroy_pipes(gameplay_t *gameplay)
{
    if (gameplay == NULL)
        return;
    for (size_t i = 0; i < gameplay->pipe_count; i++)
        pipe_destroy(&gameplay->pipes[i]);
    gameplay->pipe_count = 0;
}

void gameplay_draw_pipes(sfRenderWindow *window, const gameplay_t *gameplay)
{
    if (window == NULL || gameplay == NULL)
        return;
    for (size_t i = 0; i < gameplay->pipe_count; i++)
        pipe_draw(window, &gameplay->pipes[i]);
}
