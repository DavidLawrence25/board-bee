#include <libs/aliases.h>
#include <libs/ansi.h>
#include <libs/arena_allocator.h>
#include <libs/json.h>

#include <fstream>
#include <iostream>

using namespace rose::json;

static constexpr size_t kArenaSizeBytes = 1000 * 1000;  // 1 MB

int main(const s32 argc, const char *argv[]) {
  if (argc < 2) {
    std::cerr << "Missing JSON input and output file paths\n";
    return EXIT_FAILURE;
  }
  if (argc != 3) {
    std::cerr << "Missing JSON output file path\n";
    return EXIT_FAILURE;
  }

  auto string_allocator = mk_sptr<rose::ArenaAllocator>(kArenaSizeBytes);
  vector<Token> tokens;
  {
    std::ifstream fin(argv[1]);
    Tokenizer tokenizer(fin, string_allocator);
    tokens = tokenizer.Tokenize();
  }
  auto node_allocator = mk_sptr<rose::ArenaAllocator>(kArenaSizeBytes);
  Parser parser(std::move(tokens), node_allocator);
  parser.Parse();
  {
    std::ofstream fout(argv[2]);
    Writer writer(fout);
    writer.Write(parser.root());
  }

  return EXIT_SUCCESS;
}
