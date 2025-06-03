CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = main
SRCS = main.c Bt.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)
