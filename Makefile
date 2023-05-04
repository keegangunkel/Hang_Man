CC=gcc
CFLAGS=-g -Wall -Wextra -Wpedantic
LDFLAGS=-lm -lcurl -ljansson -lSDL2 -lSDL2_mixer
DEPS=
OBJ = lib/main.c lib/http.c lib/words.c lib/score.c lib/sound.c

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
clean:
	rm -f *.o main
install-deps:
	sudo apt-get install libcurl4-openssl-dev libjansson-dev libsdl2-dev libsdl2-mixer-dev
