/*
** EPITECH PROJECT, 2025
** My_Hunter
** File description:
** header file
*/

#ifndef MY_H_
    #define MY_H_

    #include "struct.h"

    #include <SFML/Audio.h>
    #include <SFML/Audio/Music.h>
    #include <SFML/Graphics.h>
    #include <SFML/Graphics/Rect.h>
    #include <SFML/System.h>
    #include <SFML/System/Clock.h>
    #include <SFML/Window.h>
    #include <SFML/Window/Export.h>
    #include <SFML/Window/Mouse.h>
    #include <stdlib.h>

    #define WINDOW_WIDTH 1920
    #define WINDOW_HEIGHT 1080

void leaderboard_init(leaderboard_t *leaderboard);
void leaderboard_record_score(leaderboard_t *leaderboard, int score);
void leaderboard_load(leaderboard_t *leaderboard, const char *path);
void leaderboard_save(const leaderboard_t *leaderboard, const char *path);
void leaderboard_format(const leaderboard_t *leaderboard, char *buffer,
    size_t size);

int handle_cli_arguments(int argc, char **argv);

int sprite_init(sprite_t *sprite, const char *path, const sfVector2f *pos,
    const sfVector2f *scale);
void sprite_destroy(sprite_t *sprite);
void sprite_fit_screen(sprite_t *sprite, const sfVector2u *target_size);
void sprite_set_frame(sprite_t *sprite, const sfIntRect *rect);

int pipe_init(pipe_t *pipe, const pipe_config_t *config);
void pipe_destroy(pipe_t *pipe);
void pipe_draw(sfRenderWindow *window, const pipe_t *pipe);

int mario_init(mario_t *mario);
void mario_destroy(mario_t *mario);
void mario_activate(mario_t *mario, const sfVector2f *pos);
void mario_deactivate(mario_t *mario);
void mario_set_type(mario_t *mario, runner_type_t type);
void mario_update(mario_t *mario, float delta_time);
void mario_draw(sfRenderWindow *window, const mario_t *mario);
sfBool mario_contains_point(const mario_t *mario, const sfVector2i *mouse);

int button_init(button_t *button, const button_config_t *config);
void button_set_context(button_t *button, void *context);
void button_destroy(button_t *button);
void button_draw(sfRenderWindow *window, const button_t *button);
void button_handle_click(const button_t *button, const sfVector2i *mouse);

int target_init(target_t *target);
void target_destroy(target_t *target);
void target_update(target_t *target, sfRenderWindow *window);
void target_draw(sfRenderWindow *window, const target_t *target);

int init_menu(menu_t *menu);
void destroy_menu(menu_t *menu);
void update_menu(menu_t *menu);
void draw_menu(sfRenderWindow *window, const menu_t *menu);
int menu_init_title(menu_t *menu);
void menu_destroy_title(menu_t *menu);
void menu_update_title(menu_t *menu);
void menu_handle_event(sfRenderWindow *window, menu_t *menu, state_t *state,
    const sfEvent *event);

int init_tutorial(tutorial_t *tutorial);
void destroy_tutorial(tutorial_t *tutorial);
void draw_tutorial(sfRenderWindow *window, const tutorial_t *tutorial);
void tutorial_handle_event(state_t *state);

int init_gameplay(gameplay_t *gameplay);
void destroy_gameplay(gameplay_t *gameplay);
void draw_gameplay(sfRenderWindow *window, const gameplay_t *gameplay);
void gameplay_handle_event(gameplay_t *gameplay, state_t *state,
    const sfEvent *event);
void update_gameplay(gameplay_t *gameplay);
int gameplay_init_ui(gameplay_t *gameplay);
void gameplay_destroy_ui(gameplay_t *gameplay);
void gameplay_ui_tick(gameplay_t *gameplay);
void gameplay_draw_ui(sfRenderWindow *window, const gameplay_t *gameplay);
void gameplay_reset_round(gameplay_t *gameplay);
void gameplay_add_score(gameplay_t *gameplay, int delta);
void gameplay_prepare_escape_hint(gameplay_t *gameplay);
void gameplay_format_timer_label(char *buffer, size_t size,
    float remaining_time);
void gameplay_start_round_music(gameplay_t *gameplay);
void gameplay_play_end_music(gameplay_t *gameplay);
void gameplay_stop_music(gameplay_t *gameplay);

int gameplay_init_background(gameplay_t *gameplay);
void gameplay_destroy_background(gameplay_t *gameplay);
int gameplay_init_pipes(gameplay_t *gameplay);
void gameplay_destroy_pipes(gameplay_t *gameplay);
void gameplay_draw_pipes(sfRenderWindow *window, const gameplay_t *gameplay);
int gameplay_init_mario_system(gameplay_t *gameplay);
void gameplay_destroy_mario_system(gameplay_t *gameplay);
void gameplay_spawn_runner(gameplay_t *gameplay);
void gameplay_update_runners(gameplay_t *gameplay, float delta_time);
void gameplay_handle_runner_click(gameplay_t *gameplay,
    const sfVector2i *mouse);
float gameplay_consume_delta(gameplay_t *gameplay);

#endif /* !MY_H_ */
