json_ccs := libs/json/node.cc libs/json/parser.cc libs/json/tokenizer.cc libs/json/writer.cc
misc_ccs := libs/ansi.cc
sources := src/main.cc

all: main.exe

main.exe: $(sources)
	g++ -std=c++23 -I . -L libs ${json_ccs} ${logging_ccs} ${sources} -o build/main.exe -Wall -Wpedantic -Werror

debug.exe: $(sources)
	g++ -std=c++23 -I . -L libs ${json_ccs} ${logging_ccs} ${sources} -o build/debug.exe -Wall -Wpedantic -Werror -g

clean:
	rm -f ./build/*
