#ifndef LINES
#define LINES



#include "blocks.hpp"
#include "braces.hpp"
#include "arrows.hpp"



struct MemoryByte
{
    uint32_t addr;
    uint8_t value;
    std::string name;
    bool is_pointer;
};


struct BlockLine
{
public:
    BlockLine();
    BlockLine(std::vector<MemoryByte> memoryBYTES, sf::Vector2f start, float scale,  sf::Font font, sf::Color blockCOLOR = sf::Color::Black, sf::Color topCOLOR = sf::Color::Black, sf::Color centerCOLOR = sf::Color::Black, sf::Color INVblockCOLOR = sf::Color::Red, sf::Color INVtopCOLOR = sf::Color::Red, sf::Color INVcenterCOLOR = sf::Color::Red);
    void draw(sf::RenderWindow & window);
    std::vector<Block> get_block_line();
private:
    std::vector<Block> block_line;
};


struct InfoLine
{
public:
    InfoLine();
    InfoLine(std::vector<MemoryByte> memoryBYTES, std::vector<Block> block_line, sf::Font font, sf::Color varCOLOR = sf::Color::Blue, sf::Color pointerCOLOR = sf::Color(160, 23, 194), sf::Color arrowCOLOR = sf::Color(4, 196, 4));
    void draw(sf::RenderWindow & window);
    std::vector<Horizontal_brace> get_brace_line();
    std::vector<Slip_Arrow> get_arrow_line();
private:
    std::vector<Horizontal_brace> brace_line;
    std::vector<Slip_Arrow> arrow_line;
};


#endif