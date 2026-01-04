##
## EPITECH PROJECT, 2025
## Makefile
## File description:
## Build rules
##

CC	=	clang
TARGET	=	my_hunter

SRC	=	src/core/main.c			\
		src/core/cli.c		\
		src/core/menu.c		\
		src/core/menu_title.c	\
		src/core/tutorial.c	\
		src/core/sprite_utils.c	\
		src/core/ui_button.c	\
		src/core/target.c	\
		src/gameplay/gameplay.c	\
		src/gameplay/gameplay_assets.c	\
		src/gameplay/gameplay_mario_system.c	\
		src/gameplay/gameplay_runner_spawn.c	\
		src/gameplay/gameplay_runner_events.c	\
		src/gameplay/gameplay_escape_hint.c	\
		src/gameplay/gameplay_timer_utils.c	\
		src/gameplay/gameplay_ui.c	\
		src/gameplay/gameplay_round.c	\
		src/gameplay/gameplay_time.c	\
		src/gameplay/gameplay_audio.c	\
		src/gameplay/gameplay_leaderboard.c	\
		src/gameplay/text_builder.c	\
		src/gameplay/pipe.c	\
		src/gameplay/mario.c	\
		src/gameplay/mario_state.c	\
		src/gameplay/mario_render.c

OBJ	=	$(SRC:.c=.o)

CFLAGS	=	-Wall -Wextra
CPPFLAGS=	-Iinclude
LDFLAGS	=	-lcsfml-window -lcsfml-system -lcsfml-graphics -lcsfml-audio

all:	$(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(TARGET)

re:	fclean all

valgrind: CFLAGS += -g3
valgrind: re
