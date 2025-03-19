#ifndef BLOCKS
#define BLOCKS



#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>
#include <string>



std::string UINT32toHEX(uint32_t value);


std::string UINT8toHEX(uint8_t value);


uint32_t HEX8toUINT32(std::string hex8);


struct Block
{
public:
    Block();
    Block(sf::Vector2f start, float scale, sf::Font font, uint64_t addrBYTE, uint8_t valueBYTE, sf::Color blockCOLOR = sf::Color::Black, sf::Color topCOLOR = sf::Color::Black, sf::Color centerCOLOR = sf::Color::Black);
    void draw(sf::RenderWindow & window);
    std::vector<float> get_sideSIZE();
    std::string get_valueBYTE();
    sf::Vector2f get_left_down_corner();
    sf::Vector2f get_right_down_corner();
private:
    sf::VertexArray block;
    std::vector<float> sideSIZE;
    sf::Font font;
    uint64_t addrBYTE;
    uint8_t valueBYTE;
    sf::Color blockCOLOR;
    sf::Color topCOLOR;
    sf::Color centerCOLOR;
};



#endif