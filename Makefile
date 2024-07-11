OUT := ./float
.PHONY: all run clean test

all: $(OUT)

$(OUT): emu.c
	gcc -g $^ -o $@

run: $(OUT)
	./$(OUT) $(FILE)

clean:
	rm ./$(OUT) -f

debug: $(OUT)
	gf2 --args ./$(OUT) test.rom

makeTest: $(OUT)
	./$(OUT) -

test: makeTest debug
