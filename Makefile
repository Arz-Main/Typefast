
CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lncurses

TARGET = type
SRC = type.c list.c
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)
