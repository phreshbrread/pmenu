CC = gcc
CFLAGS = -Wall -Wextra -ltinfo -lmenu -lncurses

# Run these steps even if a file exists with the same name
.PHONY: all clean

# Define object files
OBJS = obj/args.o obj/pmenu.o obj/main.o

# Objects require prepare to have been run already
$(OBJS): | prepare

# Default step when simply running 'make'
all: pmenu-linux

# Link object files into main executable
pmenu-linux: $(OBJS)
	@echo "Creating binary..."
	$(CC) $(CFLAGS) -o bin/pmenu obj/args.o obj/pmenu.o obj/main.o
	@echo "Binary created at ./bin/pmenu"

# Compile c files into object files
obj/args.o: src/args.c
	@echo "Creating object files..."
	$(CC) $(CFLAGS) -c src/args.c -o obj/args.o
obj/pmenu.o: src/pmenu.c
	$(CC) $(CFLAGS) -c src/pmenu.c -o obj/pmenu.o
obj/main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o obj/main.o

# Create build dirs
prepare:
	@echo "Creating build directories..."
	@mkdir -p bin/ obj/

# Remove build dirs
clean:
	@echo "Removing build directories..."
	@rm -r bin/ obj/
