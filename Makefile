CC = gcc
CFLAGS = -Wall -Iutils
DEBUG_FLAGS = -g -O0
FMT = indent

# Source files
SRC = shell.c interpreter.c shellmemory.c scheduler.c \
      utils/dynamicarr.c utils/pcb.c utils/queue.c utils/vector.c

# Object files (auto-convert .c → .o)
OBJ = $(SRC:.c=.o)

# -----------------------
# Default build (normal)
# -----------------------
mysh: $(OBJ)
	$(CC) $(CFLAGS) -o mysh $(OBJ)

# -----------------------
# Debug build
# -----------------------
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean mysh

# Compile rule
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

style:
	$(FMT) shell.c shell.h interpreter.c interpreter.h \
	       shellmemory.c shellmemory.h \
	       utils/dynamicarr.c utils/dynamicarr.h \
	       utils/pcb.c utils/pcb.h \
	       utils/queue.c utils/queue.h \
	       scheduler.c scheduler.h \
	       utils/vector.c utils/vector.h

clean:
	$(RM) mysh $(OBJ) *~