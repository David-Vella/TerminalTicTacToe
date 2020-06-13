DESTDIR=""

build:
	gcc -Werror -Wall src/tictactoe.c src/main.c -lncurses -o tic-tac-toe

install:
	install tic-tac-toe "$(DESTDIR)/usr/local/bin"

uninstall:
	rm "$(DESTDIR)/usr/local/bin/tic-tac-toe"

clean:
	rm tic-tac-toe

.PHONY: build, install, uninstall, clean