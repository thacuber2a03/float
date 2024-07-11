.PHONY: all run clean test

all: float

float: main.c
	gcc -g $^ -o $@

run: float
	./float $(FILE)

clean:
	rm ./float -f

debug: float
	gf2 --args ./float test.rom

makeTest: float
	./float -

test: makeTest debug
