# Lenslok

TARGET = lenskey

CC = gcc

CFLAGS = -O -Wall -Wextra
LDFLAGS = -lSDL2 -lSDL2_image

SOURCE = main.o ini.o

all:	$(SOURCE)
	$(CC) $(SOURCE) -o $(TARGET) $(LDFLAGS)

%.o:	%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)

install: all
	cp $(TARGET) ~/.local/bin/

uninstall:
	rm ~/.local/bin/$(TARGET)

# Lenslok
