CC = gcc
CFLAGS = -Wall -g

BIN = nuke

OBJS = $(patsubst src/%.c,src/%.o,$(wildcard src/*.c))

UI = ui/Nuke.ui
PY = nuke_ui.py

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) src/*.o -o $(BIN)

src/%.o: src/%.c src/%.h
	$(CC) $(CFLAGS) -c -o $@ $< $(CFLAGS)

.PHONY: gui ui clean

gui ui: $(BIN)
	pyuic5 -x $(UI) -o $(PY)

clean:
	rm -f src/*.o
	rm -f $(PY)
