/*
** EPITECH PROJECT, 2025
** My_hunter
** File description:
** leaderboard helpers
*/

#include "../../include/my.h"
#include "../../include/text_builder.h"
#include <fcntl.h>
#include <unistd.h>

static const char LEADERBOARD_FILE_PATH[] = "leaderboard.dat";

struct leaderboard_parse {
    int current;
    size_t index;
    sfBool building_number;
};

void leaderboard_init(leaderboard_t *leaderboard)
{
    if (leaderboard == NULL)
        return;
    for (size_t i = 0; i < LEADERBOARD_ENTRY_COUNT; i++)
        leaderboard->scores[i] = 0;
}

static const char *leaderboard_path_or_default(const char *path)
{
    if (path != NULL)
        return path;
    return LEADERBOARD_FILE_PATH;
}

static void leaderboard_flush_number(leaderboard_t *leaderboard,
    struct leaderboard_parse *state)
{
    if (leaderboard == NULL || state == NULL)
        return;
    if (state->building_number == sfFalse
        || state->index >= LEADERBOARD_ENTRY_COUNT)
        return;
    leaderboard->scores[state->index] = state->current;
    state->index += 1;
    state->current = 0;
    state->building_number = sfFalse;
}

static void leaderboard_handle_char(leaderboard_t *leaderboard,
    struct leaderboard_parse *state, char character)
{
    if (state == NULL || state->index >= LEADERBOARD_ENTRY_COUNT)
        return;
    if (character >= '0' && character <= '9') {
        state->current = (state->current * 10) + (character - '0');
        state->building_number = sfTrue;
        return;
    }
    leaderboard_flush_number(leaderboard, state);
}

void leaderboard_load(leaderboard_t *leaderboard, const char *path)
{
    int fd;
    char buffer[128];
    ssize_t read_bytes = 0;
    struct leaderboard_parse state = {0, 0, sfFalse};

    leaderboard_init(leaderboard);
    if (leaderboard == NULL)
        return;
    fd = open(leaderboard_path_or_default(path), O_RDONLY);
    if (fd < 0)
        return;
    while (state.index < LEADERBOARD_ENTRY_COUNT) {
        read_bytes = read(fd, buffer, sizeof(buffer));
        if (read_bytes <= 0)
            break;
        for (ssize_t i = 0; i < read_bytes; i++)
            leaderboard_handle_char(leaderboard, &state, buffer[i]);
    }
    leaderboard_flush_number(leaderboard, &state);
    close(fd);
}

static void leaderboard_write_all(int fd, const char *buffer, size_t size)
{
    size_t offset = 0;
    ssize_t written = 0;

    while (offset < size) {
        written = write(fd, buffer + offset, size - offset);
        if (written <= 0)
            return;
        offset += (size_t)written;
    }
}

void leaderboard_save(const leaderboard_t *leaderboard, const char *path)
{
    char buffer[128];
    size_t to_write;
    int fd;
    text_builder_t builder;

    if (leaderboard == NULL)
        return;
    text_builder_init(&builder, buffer, sizeof(buffer));
    for (size_t i = 0; i < LEADERBOARD_ENTRY_COUNT; i++) {
        if (i > 0)
            text_builder_push_literal(&builder, "\n");
        text_builder_push_int(&builder, leaderboard->scores[i]);
    }
    to_write = builder.index;
    fd = open(leaderboard_path_or_default(path),
        O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
        return;
    leaderboard_write_all(fd, buffer, to_write);
    close(fd);
}

static void leaderboard_shift_scores(leaderboard_t *leaderboard, size_t index)
{
    if (leaderboard == NULL)
        return;
    for (size_t shift = LEADERBOARD_ENTRY_COUNT - 1; shift > index; shift--)
        leaderboard->scores[shift] = leaderboard->scores[shift - 1];
}

void leaderboard_record_score(leaderboard_t *leaderboard, int score)
{
    if (leaderboard == NULL)
        return;
    for (size_t index = 0; index < LEADERBOARD_ENTRY_COUNT; index++) {
        if (score > leaderboard->scores[index]) {
            leaderboard_shift_scores(leaderboard, index);
            leaderboard->scores[index] = score;
            return;
        }
    }
}

void leaderboard_format(const leaderboard_t *leaderboard, char *buffer,
    size_t size)
{
    text_builder_t builder;

    if (buffer == NULL || size == 0)
        return;
    text_builder_init(&builder, buffer, size);
    if (leaderboard == NULL)
        return;
    for (size_t i = 0; i < LEADERBOARD_ENTRY_COUNT; i++) {
        if (i > 0)
            text_builder_push_literal(&builder, "\n");
        text_builder_push_int(&builder, (int)(i + 1));
        text_builder_push_literal(&builder, ". Score : ");
        text_builder_push_int(&builder, leaderboard->scores[i]);
    }
}
