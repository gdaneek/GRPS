#include "../include/blocks.hpp"



std::string UINT32toHEX(uint32_t value)
{
    std::stringstream ss;
    ss << std::hex << value;
    std::string result = ss.str();
    std::reverse(result.begin(),result.end());
    int tmp = result.size();
    for (int i = 0; i < 8 - tmp; i++)
    {
        result.push_back('0');
    }
    result.push_back('x');
    result.push_back('0');
    std::reverse(result.begin(),result.end());
    return result;
}


std::string UINT8toHEX(uint8_t value)
{
    std::stringstream ss;
    ss << "0x";
    if ((int)value < 16)
        ss << std::hex << 0;
    ss << std::hex << (int)value;
    std::string result = ss.str();
    return result;
}


uint32_t HEX8toUINT32(std::string hex8)
{
    // 00000abc -> 2748
    return std::stoul(hex8, nullptr, 16);
}


Block::Block(){}


Block::Block(sf::Vector2f start, float scale, sf::Font font, uint64_t addrBYTE, uint8_t valueBYTE, sf::Color blockCOLOR, sf::Color topCOLOR, sf::Color centerCOLOR)
{
    std::vector<float> sideSIZE{scale * 340, scale * 100};
    // Make points
    sf::Vector2f lt(start.x, start.y);
    sf::Vector2f rt(start.x + sideSIZE[0], start.y);
    sf::Vector2f rd(start.x + sideSIZE[0], start.y + sideSIZE[1]);
    sf::Vector2f ld(start.x, start.y + sideSIZE[1]);

    // Make block
    sf::VertexArray block(sf::PrimitiveType::LineStrip, 5);

    // Make block points
    block[0].position = lt;
    block[1].position = rt;
    block[2].position = rd;
    block[3].position = ld;
    block[4].position = lt;

    // Fill structure fields
    this->block = block;
    this->sideSIZE = sideSIZE;
    this->font = font;
    this->addrBYTE =addrBYTE;
    this->valueBYTE =valueBYTE;
    this->blockCOLOR = blockCOLOR;
    this->topCOLOR = topCOLOR;
    this->centerCOLOR = centerCOLOR;
}


void Block::draw(sf::RenderWindow & window)
{
    // Make block color
    for (int i = 0; i < 5; i++)
        this->block[i].color = this->blockCOLOR;

    // Make top-text
    sf::Text topText(this->font, UINT32toHEX(this->addrBYTE), this->sideSIZE[1]/2);
    topText.setPosition({this->block[0].position.x + (this->sideSIZE[0] * 0.1f), this->block[0].position.y - (this->sideSIZE[1] * 0.6f)});
    topText.setFillColor(this->topCOLOR);

    // Make center-text
    sf::Text centerText(this->font, UINT8toHEX(this->valueBYTE), this->sideSIZE[1]/2);
    centerText.setPosition({this->block[0].position.x + (this->sideSIZE[0] * 0.35f), this->block[0].position.y + (sideSIZE[1] * 0.19f)});
    centerText.setFillColor(this->centerCOLOR);

    // draw
    window.draw(this->block);
    window.draw(topText);
    window.draw(centerText);
}


std::vector<float> Block::get_sideSIZE()
{
    return this->sideSIZE;
}


std::string Block::get_valueBYTE()
{
    std::string tmp{};
    tmp.push_back(UINT8toHEX(this->valueBYTE)[2]);
    tmp.push_back(UINT8toHEX(this->valueBYTE)[3]);
    return tmp;
}


sf::Vector2f Block::get_left_down_corner()
{
    return this->block[3].position;
}


sf::Vector2f Block::get_right_down_corner()
{
    return this->block[2].position;
}