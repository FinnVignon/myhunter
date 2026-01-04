/*
** EPITECH PROJECT, 2025
** My_hunter
** File description:
** menu title animation helpers
*/

#include "../../include/my.h"

static const float TITLE_FRAME_DURATION = 0.08f;
static const sfVector2f TITLE_SCALE = {1.0f, 1.0f};
static const sfVector2f TITLE_POSITION = {WINDOW_WIDTH / 2.0f - 50, 260.0f};
static const char TITLE_TEXTURE_PATH[] = "tx/menu/mario_hunter.png";

static const sfIntRect TITLE_SOURCE_RECTS[MENU_TITLE_FRAME_COUNT] = {
    {37, 0, 727, 391},
    {836, 0, 729, 391},
    {1635, 0, 731, 391},
    {2434, 0, 733, 391},
    {3234, 0, 733, 391},
    {4033, 0, 735, 391},
    {4832, 0, 736, 391},
    {5631, 0, 738, 391},
    {6430, 0, 740, 391},
    {7229, 0, 742, 391},
    {8029, 0, 743, 391},
    {8828, 0, 745, 391},
    {9627, 0, 747, 391},
    {10427, 0, 747, 391},
    {11228, 0, 745, 391},
    {12029, 0, 743, 391},
    {12829, 0, 742, 391},
    {13630, 0, 740, 391},
    {14431, 0, 738, 391},
    {15232, 0, 736, 391},
    {16033, 0, 735, 391},
    {16834, 0, 733, 391},
    {17635, 0, 731, 391},
    {18436, 0, 729, 391},
    {19237, 0, 727, 391}
};

static void menu_setup_title_frames(menu_t *menu)
{
    if (menu == NULL)
        return;
    for (size_t i = 0; i < MENU_TITLE_FRAME_COUNT; i++)
        menu->title_frames[i] = TITLE_SOURCE_RECTS[i];
    menu->title_frame_count = MENU_TITLE_FRAME_COUNT;
    menu->title_current_frame = 0;
}

static void menu_apply_title_frame(menu_t *menu, size_t index)
{
    sfVector2f origin = {0.0f, 0.0f};

    if (menu == NULL || index >= menu->title_frame_count)
        return;
    menu->title_current_frame = index;
    if (menu->title.sprite == NULL
        || menu->title_frame_textures[index] == NULL)
        return;
    sfSprite_setTexture(menu->title.sprite,
        menu->title_frame_textures[index],
        sfTrue);
    origin.x = (float)menu->title_frames[index].width / 2.0f;
    origin.y = (float)menu->title_frames[index].height / 2.0f;
    sfSprite_setOrigin(menu->title.sprite, origin);
}

static void menu_destroy_partial_textures(menu_t *menu, size_t count)
{
    if (menu == NULL)
        return;
    for (size_t i = 0; i < count; i++) {
        if (menu->title_frame_textures[i] != NULL)
            sfTexture_destroy(menu->title_frame_textures[i]);
        menu->title_frame_textures[i] = NULL;
    }
}

static int menu_load_title_frame_texture(menu_t *menu, size_t index)
{
    sfTexture *texture;

    if (menu == NULL || index >= MENU_TITLE_FRAME_COUNT)
        return 84;
    texture = sfTexture_createFromFile(TITLE_TEXTURE_PATH,
        &menu->title_frames[index]);
    if (texture == NULL)
        return 84;
    menu->title_frame_textures[index] = texture;
    return 0;
}

static int menu_create_title_resources(menu_t *menu)
{
    menu->title.sprite = sfSprite_create();
    menu->title.texture = NULL;
    if (menu->title.sprite == NULL)
        return 84;
    sfSprite_setScale(menu->title.sprite, TITLE_SCALE);
    sfSprite_setPosition(menu->title.sprite, TITLE_POSITION);
    for (size_t i = 0; i < MENU_TITLE_FRAME_COUNT; i++) {
        if (menu_load_title_frame_texture(menu, i) == 84) {
            menu_destroy_partial_textures(menu, i);
            return 84;
        }
    }
    return 0;
}

int menu_init_title(menu_t *menu)
{
    if (menu == NULL)
        return 84;
    menu_setup_title_frames(menu);
    if (menu_create_title_resources(menu) == 84)
        return 84;
    menu->title_clock = sfClock_create();
    if (menu->title_clock == NULL)
        return 84;
    menu_apply_title_frame(menu, 0);
    sfClock_restart(menu->title_clock);
    return 0;
}

void menu_destroy_title(menu_t *menu)
{
    if (menu == NULL)
        return;
    sprite_destroy(&menu->title);
    menu_destroy_partial_textures(menu, MENU_TITLE_FRAME_COUNT);
    if (menu->title_clock != NULL)
        sfClock_destroy(menu->title_clock);
    menu->title_clock = NULL;
}

void menu_update_title(menu_t *menu)
{
    sfTime elapsed;
    float seconds;
    size_t next_frame;

    if (menu == NULL || menu->title_clock == NULL
        || menu->title.sprite == NULL
        || menu->title_frame_count == 0)
        return;
    elapsed = sfClock_getElapsedTime(menu->title_clock);
    seconds = sfTime_asSeconds(elapsed);
    if (seconds < TITLE_FRAME_DURATION)
        return;
    next_frame = (menu->title_current_frame + 1) % menu->title_frame_count;
    menu_apply_title_frame(menu, next_frame);
    sfClock_restart(menu->title_clock);
}
