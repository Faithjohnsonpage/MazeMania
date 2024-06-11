# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Werror -Wextra -pedantic

# SDL2 flags
SDL2_CFLAGS = $(shell sdl2-config --cflags)
SDL2_LDFLAGS = $(shell sdl2-config --libs) -lSDL2_image -lm

# Source files
SRCS = castRays_exe.c checkIntersection.c draw_functions.c enemy_handling.c levelManager.c loadTextures.c loadWallTexture.c load_Worlds.c main.c renderScreen.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable
EXEC = main

# Default target
all: $(EXEC)

# Build the executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC) $(SDL2_LDFLAGS)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) $(SDL2_CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(EXEC)

# Phony targets
.PHONY: all clean
