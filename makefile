CC = gcc
CFLAGS = -Wall -Wextra -ltinfo -lmenu -lncurses

# Run these steps even if a file exists with the same name
.PHONY: all clean

# Define object files
OBJS = obj/args.o obj/pmenu.o obj/main.o

# Default step when simply running 'make'
all: pmenu-linux

# Link object files into main executable
pmenu-linux: prepare $(OBJS)
	@echo "Creating object files..."
	@echo "Creating binary..."
	$(CC) $(CFLAGS) -o bin/pmenu $(OBJS)
	@echo "Binary created at ./bin/pmenu"

# Compile c files into object files
obj/args.o: src/args.c
	$(CC) $(CFLAGS) -c src/args.c -o obj/args.o
obj/pmenu.o: src/pmenu.c
	$(CC) $(CFLAGS) -c src/pmenu.c -o obj/pmenu.o
obj/main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o obj/main.o

# Create build dirs
prepare:
	@echo "Creating build directories..."
	@mkdir -p bin/ obj/

install: pmenu-linux
	install -m 755 bin/pmenu /usr/local/bin/pmenu
	@echo "Installed to /usr/local/bin/pmenu"

install-local: pmenu-linux
	mkdir -p ~/.local/bin/
	install -m 755 bin/pmenu ~/.local/bin/pmenu
	@echo "Installed to ~/.local/bin/pmenu"

# Remove build dirs
clean:
	@echo "Removing build directories..."
	@rm -r bin/ obj/
