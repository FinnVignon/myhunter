/*
** EPITECH PROJECT, 2025
** My_hunter
** File description:
** command line interface helpers
*/

#include "../../include/my.h"
#include <unistd.h>

static int print_help_message(void)
{
    static const char HELP_MESSAGE[] =
        "USAGE\n"
        "    ./my_hunter [-h]\n"
        "\n"
        "DESCRIPTION\n"
        "    Mario Hunter is a Duck Hunt inspired shooter where you try to\n"
        "    stop runners from escaping their pipes.\n"
        "\n"
        "CONTROLS\n"
        "    Left Click: shoot runners on screen.\n"
        "    Space: continue past the tutorial screen.\n"
        "    Escape: return to the main menu while playing.\n"
        "    Menu Buttons: click Start to play, Exit to quit the game.\n";

    if (write(STDOUT_FILENO, HELP_MESSAGE, sizeof(HELP_MESSAGE) - 1) < 0)
        return 84;
    return 0;
}

int handle_cli_arguments(int argc, char **argv)
{
    static const char ERROR_MESSAGE[] =
        "Error: invalid arguments. Use -h for help.\n";

    if (argc == 1)
        return 0;
    if (argc == 2 && argv != NULL && argv[1] != NULL
        && argv[1][0] == '-' && argv[1][1] == 'h' && argv[1][2] == '\0') {
        if (print_help_message() == 84)
            return -1;
        return 1;
    }
    write(STDERR_FILENO, ERROR_MESSAGE, sizeof(ERROR_MESSAGE) - 1);
    return -1;
}
