/*
** EPITECH PROJECT, 2025
** My_hunter
** File description:
** gameplay round management helpers
*/

#include "../../include/my.h"
#include "../../include/text_builder.h"

static const sfVector2f SCORE_TEXT_POSITION = {60.0f, 30.0f};
static const sfVector2f TIMER_TEXT_POSITION = {WINDOW_WIDTH - 100.0f, 30.0f};
static const sfVector2f END_TEXT_POSITION = {WINDOW_WIDTH / 2.0f,
    WINDOW_HEIGHT / 2.0f};
static const float ESCAPE_BLINK_INTERVAL = 0.6f;
static const size_t ESCAPE_CLOCK_COUNT = 5;
static const size_t END_TEXT_BUFFER_SIZE = 192;
static const size_t LEADERBOARD_BUFFER_SIZE = 96;

static void gameplay_place_score_text(gameplay_t *gameplay)
{
    sfFloatRect bounds;
    sfVector2f origin = {0.0f, 0.0f};
    char buffer[32];
    text_builder_t builder;

    if (gameplay == NULL || gameplay->score_text == NULL)
        return;
    text_builder_init(&builder, buffer, sizeof(buffer));
    text_builder_push_literal(&builder, "Score: ");
    text_builder_push_int(&builder, gameplay->score);
    sfText_setString(gameplay->score_text, buffer);
    bounds = sfText_getLocalBounds(gameplay->score_text);
    origin.x = bounds.left;
    origin.y = bounds.top;
    sfText_setOrigin(gameplay->score_text, origin);
    sfText_setPosition(gameplay->score_text, SCORE_TEXT_POSITION);
}

static void gameplay_place_timer_text(gameplay_t *gameplay,
    float remaining_time)
{
    char buffer[16];
    sfFloatRect bounds;
    sfVector2f origin;

    if (gameplay == NULL || gameplay->timer_text == NULL)
        return;
    gameplay_format_timer_label(buffer, sizeof(buffer), remaining_time);
    sfText_setString(gameplay->timer_text, buffer);
    bounds = sfText_getLocalBounds(gameplay->timer_text);
    origin.x = bounds.left + bounds.width;
    origin.y = bounds.top;
    sfText_setOrigin(gameplay->timer_text, origin);
    sfText_setPosition(gameplay->timer_text, TIMER_TEXT_POSITION);
}

static void gameplay_build_end_buffer(const gameplay_t *gameplay,
    text_builder_t *builder, const char *message, char *leaderboard_buffer)
{
    if (gameplay == NULL || builder == NULL)
        return;
    text_builder_push_literal(builder, message);
    text_builder_push_literal(builder, "\nScore: ");
    text_builder_push_int(builder, gameplay->score);
    text_builder_push_literal(builder, "\n\n");
    leaderboard_format(&gameplay->leaderboard, leaderboard_buffer,
        LEADERBOARD_BUFFER_SIZE);
    text_builder_push_literal(builder, leaderboard_buffer);
}

static void gameplay_set_end_message(gameplay_t *gameplay,
    const char *label)
{
    sfFloatRect bounds;
    char buffer[END_TEXT_BUFFER_SIZE];
    char leaderboard_buffer[LEADERBOARD_BUFFER_SIZE];
    const char *message = (label != NULL) ? label : "Game Over!";
    text_builder_t builder;

    if (gameplay == NULL || gameplay->end_text == NULL)
        return;
    text_builder_init(&builder, buffer, sizeof(buffer));
    gameplay_build_end_buffer(gameplay, &builder, message, leaderboard_buffer);
    sfText_setString(gameplay->end_text, buffer);
    bounds = sfText_getLocalBounds(gameplay->end_text);
    sfText_setOrigin(gameplay->end_text, (sfVector2f){
            bounds.left + bounds.width / 2.0f,
            bounds.top + bounds.height / 2.0f});
    sfText_setPosition(gameplay->end_text, END_TEXT_POSITION);
}

static void gameplay_finish_round(gameplay_t *gameplay,
    const char *label)
{
    if (gameplay == NULL || gameplay->round_over == sfTrue)
        return;
    gameplay->round_over = sfTrue;
    gameplay_play_end_music(gameplay);
    leaderboard_record_score(&gameplay->leaderboard, gameplay->score);
    leaderboard_save(&gameplay->leaderboard, NULL);
    gameplay_set_end_message(gameplay, label);
    gameplay_prepare_escape_hint(gameplay);
    gameplay->escape_visible = sfTrue;
    if (gameplay->escape_clock != NULL)
        sfClock_restart(gameplay->escape_clock);
    for (size_t i = 0; i < MARIO_MAX_COUNT; i++)
        mario_deactivate(&gameplay->marios[i]);
}

static void gameplay_restart_round_resources(gameplay_t *gameplay)
{
    sfClock *clocks[ESCAPE_CLOCK_COUNT];

    if (gameplay == NULL)
        return;
    clocks[0] = gameplay->escape_clock;
    clocks[1] = gameplay->round_clock;
    clocks[2] = gameplay->mario_spawn_clock;
    clocks[3] = gameplay->delta_clock;
    clocks[4] = gameplay->enemy_spawn_clock;
    for (size_t i = 0; i < ESCAPE_CLOCK_COUNT; i++) {
        if (clocks[i] != NULL)
            sfClock_restart(clocks[i]);
    }
    for (size_t i = 0; i < MARIO_MAX_COUNT; i++)
        mario_deactivate(&gameplay->marios[i]);
}

void gameplay_reset_round(gameplay_t *gameplay)
{
    if (gameplay == NULL)
        return;
    gameplay->score = 0;
    gameplay->round_over = sfFalse;
    gameplay->timer_expired = sfFalse;
    gameplay_place_score_text(gameplay);
    gameplay_place_timer_text(gameplay, GAME_ROUND_DURATION);
    if (gameplay->end_text != NULL) {
        sfText_setString(gameplay->end_text, "");
        sfText_setOrigin(gameplay->end_text, (sfVector2f){0.0f, 0.0f});
        sfText_setPosition(gameplay->end_text, END_TEXT_POSITION);
    }
    if (gameplay->escape_text != NULL)
        sfText_setString(gameplay->escape_text, "");
    gameplay->escape_visible = sfFalse;
    gameplay_restart_round_resources(gameplay);
}

static void gameplay_handle_timer(gameplay_t *gameplay)
{
    sfTime elapsed;
    float seconds;
    float remaining;

    if (gameplay == NULL || gameplay->round_clock == NULL
        || gameplay->round_over == sfTrue)
        return;
    elapsed = sfClock_getElapsedTime(gameplay->round_clock);
    seconds = sfTime_asSeconds(elapsed);
    remaining = GAME_ROUND_DURATION - seconds;
    gameplay_place_timer_text(gameplay, remaining);
    if (remaining <= 0.0f) {
        gameplay->timer_expired = sfTrue;
        gameplay_place_timer_text(gameplay, 0.0f);
        gameplay_finish_round(gameplay, "Time's Up!");
    }
}

void gameplay_ui_tick(gameplay_t *gameplay)
{
    sfTime elapsed;
    float seconds;

    gameplay_handle_timer(gameplay);
    if (gameplay == NULL || gameplay->escape_clock == NULL
        || gameplay->escape_text == NULL || gameplay->round_over == sfFalse)
        return;
    elapsed = sfClock_getElapsedTime(gameplay->escape_clock);
    seconds = sfTime_asSeconds(elapsed);
    if (seconds < ESCAPE_BLINK_INTERVAL)
        return;
    gameplay->escape_visible = (gameplay->escape_visible == sfFalse)
        ? sfTrue : sfFalse;
    sfClock_restart(gameplay->escape_clock);
}

void gameplay_add_score(gameplay_t *gameplay, int delta)
{
    if (gameplay == NULL || gameplay->round_over == sfTrue)
        return;
    gameplay->score += delta;
    gameplay_place_score_text(gameplay);
    if (gameplay->score < 0) {
        gameplay->timer_expired = sfFalse;
        gameplay_finish_round(gameplay, "Game Over!");
    }
}
