all: pkg-set

pkg-set: src/main.cpp
	g++ -o pkg-set src/main.cpp

install: pkg-set
	cp pkg-set /usr/local/bin
	make clean
clean:
	rm -f pkg-set

remove:
	rm -f /usr/local/bin/pkg-set
	make clean
.PHONY: install remove
