#include "include/memory_vizual.hpp"


int main(int argc, char const *argv[])
{
    std::vector<MemoryByte> memoryBYTES{
        {0x01, 0xa3, "x", false},
        {0x02, 0x0e, "x", false},
        {0x03, 0x12, "x", false},
        {0x04, 0x34, "x", false },
        {0x05, 0x4f, "", false},
        {0x06, 0xfe, "", false},
        {0x07, 0xcf, "", false},
        {0x08, 0xbc, "", false},
        {0x09, 0x00, "px", true},
        {0x0a, 0x00, "px", true},
        {0x0b, 0x00, "px", true},
        {0x0c, 0x01, "px", true},
        {0x0d, 0xff, "y", false},
        {0x0e, 0xf2, "y", false},
        {0x0f, 0x0d, "y", false},
        {0x10, 0xdd, "y", false },
        {0x11, 0xce, "", false},
        {0x12, 0xf4, "", false},
        {0x13, 0xcc, "", false},
        {0x14, 0xb2, "", false},
        {0x15, 0x00, "ppy", true},
        {0x16, 0x00, "ppy", true},
        {0x17, 0x00, "ppy", true},
        {0x18, 0x19, "ppy", true},
        {0x19, 0x00, "py", true},
        {0x1a, 0x00, "py", true},
        {0x1b, 0x00, "py", true},
        {0x1c, 0x0d, "py", true},
        {0x1d, 0xc0, "", false},
        {0x1e, 0xfd, "", false},
        {0x1f, 0x41, "", false},
        {0x20, 0x00, "ppx", true},
        {0x21, 0x00, "ppx", true},
        {0x22, 0x00, "ppx", true},
        {0x23, 0x09, "ppx", true},
        {0x24, 0xef, "", false},
        };
    sf::Font font("../resources/fonts/arial.ttf");
    memoryVIZUALIZATION(memoryBYTES, font);
    return 0;
}
