CC = gcc

CFLAGS = -O2 -Wall -Wextra
LDFLAGS = -lm

PROG = solve
SOURCE = main.c

all: $(PROG)

$(PROG): $(SOURCE)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	strip -s $@

clean:
	rm -f $(PROG)
