all: pkg-set

pkg-set: main.cpp
	g++ -o pkg-set src/main.cpp

install: pkg-set
	cp pkg-set /usr/local/bin

remove:
	rm /usr/local/bin/pkg-set

.PHONY: install remove
