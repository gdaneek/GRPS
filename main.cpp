#include "include/memory_vizual.hpp"
#include "include/tmp.hpp"
#include "tokenizer.hpp"

using namespace tmp;

int main(int argc, char const *argv[]) {

    sf::Font font("../resources/fonts/arial.ttf");

    std::string fpath;

    std::cout << "enter the name of the source code file: ";
    std::cin >> fpath;

    std::ifstream file(fpath);
    if (!file.is_open()) {
        std::cerr << "File not found!" << std::endl;
        return 1;
    }

    std::vector<std::string> tokens;

    FileToTokensConverter converter;
    converter.f(file, tokens);          // text to tokens

    TMP tmp;
    auto memory = tmp.process(tokens);  // tokens to MemoryPart

    memoryVIZUALIZATION(memory, font);  // vVsualize MemoryPart

    return 0;
}
