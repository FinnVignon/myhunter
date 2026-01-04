/*
** EPITECH PROJECT, 2025
** My_hunter
** File description:
** menu screen management
*/

#include "../../include/my.h"

static void start_button_callback(void *context)
{
    state_t *state = context;

    if (state != NULL)
        *state = STATE_TUTORIAL;
}

static void exit_button_callback(void *context)
{
    sfRenderWindow *window = context;

    if (window != NULL)
        sfRenderWindow_close(window);
}

static int init_background(menu_t *menu)
{
    const sfVector2f pos = {0.0f, 0.0f};
    const sfVector2f scale = {1.0f, 1.0f};

    return sprite_init(&menu->background, "tx/menu/menubk.jpg", &pos, &scale);
}

static int init_buttons(menu_t *menu)
{
    static const button_config_t start_config = {
        .texture_path = "tx/menu/menubs.png",
        .position = {760.0f, 450.0f},
        .scale = {1.2f, 1.2f},
        .callback = &start_button_callback
    };
    static const button_config_t exit_config = {
        .texture_path = "tx/menu/menubx.png",
        .position = {760.0f, 650.0f},
        .scale = {1.2f, 1.2f},
        .callback = &exit_button_callback
    };

    if (button_init(&menu->start_button, &start_config) == 84)
        return 84;
    if (button_init(&menu->exit_button, &exit_config) == 84)
        return 84;
    return 0;
}

static int init_menu_music(menu_t *menu)
{
    menu->music = sfMusic_createFromFile("./audio/menu.mp3");
    if (menu->music == NULL)
        return 84;
    sfMusic_setLoop(menu->music, sfTrue);
    return 0;
}

int init_menu(menu_t *menu)
{
    menu->title_clock = NULL;
    for (size_t i = 0; i < MENU_TITLE_FRAME_COUNT; i++)
        menu->title_frame_textures[i] = NULL;
    menu->music = NULL;
    if (init_background(menu) == 84)
        return 84;
    if (init_buttons(menu) == 84) {
        destroy_menu(menu);
        return 84;
    }
    if (menu_init_title(menu) == 84) {
        destroy_menu(menu);
        return 84;
    }
    if (init_menu_music(menu) == 84) {
        destroy_menu(menu);
        return 84;
    }
    return 0;
}

void destroy_menu(menu_t *menu)
{
    sprite_destroy(&menu->background);
    button_destroy(&menu->start_button);
    button_destroy(&menu->exit_button);
    menu_destroy_title(menu);
    if (menu->music != NULL)
        sfMusic_destroy(menu->music);
    menu->music = NULL;
}

void update_menu(menu_t *menu)
{
    menu_update_title(menu);
}

void draw_menu(sfRenderWindow *window, const menu_t *menu)
{
    sfRenderWindow_drawSprite(window, menu->background.sprite, NULL);
    if (menu->title.sprite != NULL)
        sfRenderWindow_drawSprite(window, menu->title.sprite, NULL);
    button_draw(window, &menu->start_button);
    button_draw(window, &menu->exit_button);
}

void menu_handle_event(sfRenderWindow *window, menu_t *menu, state_t *state,
    const sfEvent *event)
{
    sfVector2i mouse;

    if (event->type != sfEvtMouseButtonPressed)
        return;
    mouse = sfMouse_getPositionRenderWindow(window);
    button_set_context(&menu->start_button, state);
    button_handle_click(&menu->start_button, &mouse);
    button_set_context(&menu->exit_button, window);
    button_handle_click(&menu->exit_button, &mouse);
}
