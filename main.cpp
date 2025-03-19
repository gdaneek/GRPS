#include "include/memory_vizual.hpp"
#include "include/t2mp.hpp"
#include "tokenizer.hpp"

using namespace t2mp;

int main(int argc, char const *argv[]) {

    sf::Font font("../resources/fonts/arial.ttf");

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "File not found!" << std::endl;
        return 1;
    }

    FileToTokensConverter converter;
    std::vector<std::string> tokens;
    converter.f(file, tokens);

    T2MP t2mp;
    auto memory = t2mp.process(tokens);

    // Bad . Other variants ?
    std::vector<MemoryByte> expected;
    for(auto x : memory) expected.push_back(*reinterpret_cast<MemoryByte*>(&x));
    memoryVIZUALIZATION(expected, font);
    return 0;
}
