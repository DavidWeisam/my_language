# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c11 -g

# Source files
SRCS = main_program.c lexer.c parser.c

# Header files
HDRS = lexer.h parser.h

# Object files
OBJS = $(SRCS:.c=.o)

# Output executable
TARGET = dudu

# Default target
all: $(TARGET)

# Rule to build the target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to build object files
%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean the build
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
