CC = gcc
CFLAGS = -Wall -g -std=gnu11

INSTALL_PATH = /usr/local/bin
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

install:
	install -m 755 $(BIN) $(INSTALL_PATH)/$(BIN)

uninstall:
	rm -f $(INSTALL_PATH)/$(BIN)

clean:
	rm -f $(BIN)
	rm -f src/*.o
	rm -f $(PY)
