CC = gcc
CFLAGS = -Wall -Iutils
FMT = indent

# Source files
SRC = shell.c interpreter.c shellmemory.c \
      utils/dynamicarr.c utils/pcb.c utils/queue.c scheduler.c

# Object files (auto-convert .c → .o)
OBJ = $(SRC:.c=.o)

# Target
mysh: $(OBJ)
	$(CC) $(CFLAGS) -o mysh $(OBJ)

# Compile rule
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

style:
	$(FMT) shell.c shell.h interpreter.c interpreter.h \
	       shellmemory.c shellmemory.h \
	       utils/dynamicarr.c utils/dynamicarr.h \
	       utils/pcb.c utils/pcb.h \
	       utils/queue.c utils/queue.h \
		   scheduler.c scheduler.h

clean:
	$(RM) mysh $(OBJ) *~