sources := libs/json/parser.cc libs/json/tokenizer.cc libs/json/writer.cc src/main.cc

all: main.exe

main.exe: ${sources}
	g++ -std=c++23 -I . -L libs ${sources} -o build/main.exe -Wall -Wpedantic -Werror

debug.exe: ${sources}
	g++ -std=c++23 -I . -L libs ${sources} -o build/debug.exe -Wall -Wpedantic -Werror -g

clean:
	rm -f ./build/*
