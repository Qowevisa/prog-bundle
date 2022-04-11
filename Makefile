path_f = /usr/local/bin
SRC = src
BIN = bin
programs_src = $(shell ls $(SRC) | grep .c)
programs = $(foreach prog,$(programs_src),$(shell echo $(prog) | cut -d '.' -f 1))
programs_b = $(foreach prog,$(programs),$(BIN)/$(prog))
CFLAGS = -Wall -Wextra -Werror -pedantic -O2 -g

def:
	@echo "Install any of these programs with \"make [PROGRAM]\" command!"
	@echo "Programs:"
	@echo $(programs)

all: $(programs)
	@

uninstall:
	sudo rm $(foreach prog,$(programs),$(path_f)/$(prog))

$(BIN):
	mkdir $(BIN)

$(BIN)/% : $(SRC)/%.c
	gcc -o $@ $< $(CFLAGS)

compile-all: $(BIN) $(programs:%=$(BIN)/%)
	@

clean: $(BIN)
	rm $(BIN)/*

compile-and-send: $(path_f) compile-all
	sudo cp $(BIN)/* $(path_f)

$(path_f):
	@echo "Seems like you don't have $(path_f) folder! Enter sudo password to create it!"
	sudo mkdir -p $(path_f)

$(programs): $(path_f)
	sudo gcc $(SRC)/$@.c -o $(path_f)/$@ $(CFLAGS)

.PHONY: compile-all uninstall all clean
