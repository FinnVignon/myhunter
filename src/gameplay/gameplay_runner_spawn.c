/*
** EPITECH PROJECT, 2025
** My_hunter
** File description:
** gameplay runner spawn orchestration
*/

#include "../../include/my.h"

static const float MARIO_PIPE_HALF = (PIPE_BASE_HEIGHT * PIPE_SCALE_VALUE)
    / 2.0f;
static const float MARIO_SPAWN_X = MARIO_PIPE_HALF - 120.0f;
static const float MARIO_ENTRY_Y[] = {270.0f, 490.0f, 710.0f};
static const size_t MARIO_ENTRY_COUNT =
    sizeof(MARIO_ENTRY_Y) / sizeof(MARIO_ENTRY_Y[0]);
static const float VERTICAL_PIPE_X[] = {460.0f, 760.0f, 1060.0f, 1360.0f};
static const size_t VERTICAL_PIPE_COUNT =
    sizeof(VERTICAL_PIPE_X) / sizeof(VERTICAL_PIPE_X[0]);
static const float TOP_PIPE_CENTER_Y = MARIO_PIPE_HALF - 60.0f;
static const float BOTTOM_PIPE_CENTER_Y = WINDOW_HEIGHT - MARIO_PIPE_HALF
    + 70.0f;
static const float GOOMBA_SPAWN_Y = TOP_PIPE_CENTER_Y + 110.0f;
static const float KOOPA_SPAWN_Y = BOTTOM_PIPE_CENTER_Y - 90.0f;
static const float GOOMBA_EXIT_Y = BOTTOM_PIPE_CENTER_Y - 140.0f;
static const float KOOPA_EXIT_Y = TOP_PIPE_CENTER_Y + 140.0f;
static const float GOOMBA_FALL_SPEED = 220.0f;
static const float KOOPA_ASCEND_SPEED = 240.0f;
static const float MARIO_EXIT_X = WINDOW_WIDTH
    - ((PIPE_BASE_HEIGHT * PIPE_SCALE_VALUE) / 2.0f) - 12.0f;

static float gameplay_compute_interval(const gameplay_t *gameplay, float base,
    float per_point, float min_value)
{
    float interval = base;

    if (gameplay != NULL)
        interval -= (float)gameplay->score * per_point;
    if (interval < min_value)
        interval = min_value;
    return interval;
}

static float gameplay_compute_runner_speed(const gameplay_t *gameplay)
{
    float speed = MARIO_BASE_SPEED;

    if (gameplay != NULL)
        speed += (float)gameplay->score * MARIO_SPEED_PER_POINT;
    if (speed < MARIO_MIN_SPEED)
        speed = MARIO_MIN_SPEED;
    if (speed > MARIO_MAX_SPEED)
        speed = MARIO_MAX_SPEED;
    return speed;
}

static mario_t *gameplay_acquire_runner(gameplay_t *gameplay)
{
    if (gameplay == NULL)
        return NULL;
    for (size_t i = 0; i < MARIO_MAX_COUNT; i++) {
        if (gameplay->marios[i].active == sfFalse)
            return &gameplay->marios[i];
    }
    return NULL;
}

static void gameplay_configure_mario(gameplay_t *gameplay, mario_t *runner)
{
    sfVector2f spawn;
    size_t entry_index;

    if (runner == NULL)
        return;
    entry_index = (size_t)(rand() % (int)MARIO_ENTRY_COUNT);
    spawn.x = MARIO_SPAWN_X;
    spawn.y = MARIO_ENTRY_Y[entry_index];
    mario_set_type(runner, RUNNER_TYPE_MARIO);
    runner->velocity.x = gameplay_compute_runner_speed(gameplay);
    runner->velocity.y = 0.0f;
    runner->hit_score_delta = 1;
    runner->escape_score_delta = -1;
    runner->exit_limit = MARIO_EXIT_X;
    runner->exit_vertical = sfFalse;
    runner->exit_reverse = sfFalse;
    mario_activate(runner, &spawn);
}

static void gameplay_configure_goomba(mario_t *runner)
{
    size_t index;
    sfVector2f spawn;

    if (runner == NULL)
        return;
    index = (size_t)(rand() % (int)VERTICAL_PIPE_COUNT);
    spawn.x = VERTICAL_PIPE_X[index];
    spawn.y = GOOMBA_SPAWN_Y;
    mario_set_type(runner, RUNNER_TYPE_GOOMBA);
    runner->velocity.x = 0.0f;
    runner->velocity.y = GOOMBA_FALL_SPEED;
    runner->hit_score_delta = -2;
    runner->escape_score_delta = 0;
    runner->exit_limit = GOOMBA_EXIT_Y;
    runner->exit_vertical = sfTrue;
    runner->exit_reverse = sfFalse;
    mario_activate(runner, &spawn);
}

static void gameplay_configure_koopa(mario_t *runner)
{
    size_t index;
    sfVector2f spawn;

    if (runner == NULL)
        return;
    index = (size_t)(rand() % (int)VERTICAL_PIPE_COUNT);
    spawn.x = VERTICAL_PIPE_X[index];
    spawn.y = KOOPA_SPAWN_Y;
    mario_set_type(runner, RUNNER_TYPE_KOOPA);
    runner->velocity.x = 0.0f;
    runner->velocity.y = -KOOPA_ASCEND_SPEED;
    runner->hit_score_delta = -5;
    runner->escape_score_delta = 0;
    runner->exit_limit = KOOPA_EXIT_Y;
    runner->exit_vertical = sfTrue;
    runner->exit_reverse = sfTrue;
    mario_activate(runner, &spawn);
}

static void gameplay_spawn_runner_kind(gameplay_t *gameplay, runner_type_t type)
{
    mario_t *target = gameplay_acquire_runner(gameplay);

    if (target == NULL)
        return;
    if (type == RUNNER_TYPE_GOOMBA) {
        gameplay_configure_goomba(target);
        return;
    }
    if (type == RUNNER_TYPE_KOOPA) {
        gameplay_configure_koopa(target);
        return;
    }
    gameplay_configure_mario(gameplay, target);
}

static void gameplay_try_spawn_mario(gameplay_t *gameplay)
{
    sfTime elapsed;

    if (gameplay == NULL || gameplay->mario_spawn_clock == NULL
        || gameplay->round_over == sfTrue)
        return;
    elapsed = sfClock_getElapsedTime(gameplay->mario_spawn_clock);
    if (sfTime_asSeconds(elapsed)
        < gameplay_compute_interval(gameplay, MARIO_SPAWN_INTERVAL,
            MARIO_SPAWN_DELTA_PER_POINT, MARIO_MIN_SPAWN_INTERVAL))
        return;
    gameplay_spawn_runner_kind(gameplay, RUNNER_TYPE_MARIO);
    sfClock_restart(gameplay->mario_spawn_clock);
}

static void gameplay_try_spawn_enemy(gameplay_t *gameplay)
{
    sfTime elapsed;
    runner_type_t type;

    if (gameplay == NULL || gameplay->enemy_spawn_clock == NULL
        || gameplay->round_over == sfTrue)
        return;
    elapsed = sfClock_getElapsedTime(gameplay->enemy_spawn_clock);
    if (sfTime_asSeconds(elapsed)
        < gameplay_compute_interval(gameplay, ENEMY_SPAWN_INTERVAL,
            ENEMY_SPAWN_DELTA_PER_POINT, ENEMY_MIN_SPAWN_INTERVAL))
        return;
    type = (rand() % 2 == 0) ? RUNNER_TYPE_GOOMBA : RUNNER_TYPE_KOOPA;
    gameplay_spawn_runner_kind(gameplay, type);
    sfClock_restart(gameplay->enemy_spawn_clock);
}

void gameplay_spawn_runner(gameplay_t *gameplay)
{
    gameplay_try_spawn_mario(gameplay);
    gameplay_try_spawn_enemy(gameplay);
}
