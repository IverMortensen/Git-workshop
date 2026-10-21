SRCS = $(wildcard plings/*.c) patchlings.c

LDFLAGS = -lSDL3 -lSDL3_ttf -Iplings
CFLAGS = -Wall -Werror -Wextra -std=c89 --pedantic -O2


TARGET = patchlings

all:
	gcc $(SRCS) $(LDFLAGS) $(CFLAGS) -o $(TARGET)