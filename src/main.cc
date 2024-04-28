#include <libs/aliases.h>
#include <libs/json.h>

#include <fstream>
#include <iostream>

using namespace rose::json;

int main(const s32 argc, const char *argv[]) {
  if (argc != 2) {
    std::cerr << "Missing JSON file path\n";
    return EXIT_FAILURE;
  }

  Tokenizer tokenizer(argv[1]);
  Parser parser(tokenizer.Tokenize());
  parser.Parse();

  std::cout << "break me :3\n";
  return EXIT_SUCCESS;
}
