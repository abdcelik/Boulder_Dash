CC := gcc

CFLAGS := -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

HDRS := ./Sources/BD_basis.h ./Sources/BD_game.c ./Sources/BD_animation.h

SRCS := ./Sources/main.c ./Sources/BD_basis.c ./Sources/BD_game.c ./Sources/BD_animation.c

OBJS := $(SRCS:.c=.o)

EXEC := BoulderDash

all: $(EXEC)

$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS)
