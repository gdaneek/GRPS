#include "../include/lines.hpp"



BlockLine::BlockLine()
{
}


BlockLine::BlockLine(std::vector<MemoryByte> memoryBYTES, sf::Vector2f start, float scale, sf::Font font, sf::Color blockCOLOR, sf::Color topCOLOR, sf::Color centerCOLOR, sf::Color INVblockCOLOR, sf::Color INVtopCOLOR, sf::Color INVcenterCOLOR)
{
    std::vector<Block> block_line{};
    for (int i = 0; i < memoryBYTES.size(); i++)
    {
        if (memoryBYTES[i].name != "")
        {
            Block block({start.x + (i * scale * 340) + i, start.y}, scale, font, memoryBYTES[i].addr, memoryBYTES[i].value, INVblockCOLOR, INVtopCOLOR, INVcenterCOLOR);
            block_line.push_back(block);
        }
        else
        {
            Block block({start.x + (i * scale * 340) + i, start.y}, scale, font,  memoryBYTES[i].addr, memoryBYTES[i].value, blockCOLOR, topCOLOR, centerCOLOR);
            block_line.push_back(block);
        }
    }
    this->block_line = block_line;
}


void BlockLine::draw(sf::RenderWindow & window)
{
    for (int i = 0; i < this->block_line.size(); i++)
    {
            this->block_line[i].draw(window);
    }
}


std::vector<Block> BlockLine::get_block_line()
{
    return this->block_line;
}



InfoLine::InfoLine()
{}


InfoLine::InfoLine(std::vector<MemoryByte> memoryBYTES, std::vector<Block> block_line, sf::Font font, sf::Color varCOLOR, sf::Color pointerCOLOR, sf::Color arrowCOLOR)
{
    std::vector<Horizontal_brace> brace_line{};
    std::vector<Slip_Arrow> arrow_line{};
    std::vector<sf::Vector2f> arrow_start_line{};
    std::vector<sf::Vector2f> arrow_end_line{};
    std::vector<bool> is_pointer_line{};
    std::string tmp_name{};
    std::string tmp_hex_addr{};
    bool tmp_is_pointer = false;
    uint32_t base_addr = memoryBYTES[0].addr;
    uint32_t tmp_start{};
    uint32_t tmp_addr{};
    float shift_up = block_line[0].get_sideSIZE()[1] * 0.5f;
    for (uint32_t i = 0; i < memoryBYTES.size(); i++)
    {
        tmp_name = memoryBYTES[i].name;
        tmp_is_pointer = memoryBYTES[i].is_pointer;
        tmp_start = i;
        for (uint32_t j = i; j < memoryBYTES.size(); j++)
        {
            if (memoryBYTES[j].name == tmp_name)
            {
                tmp_hex_addr += block_line[j].get_valueBYTE();
                if (j == memoryBYTES.size() - 1)
                {
                    i = j;
                    break;
                }
            }
            else
            {
                i = j - 1;
                break;
            }
        }
        if (tmp_name != "")
        {
            if (tmp_is_pointer)
            {
                tmp_addr = HEX8toUINT32(tmp_hex_addr);
                Horizontal_brace brace(block_line[tmp_start].get_left_down_corner(), block_line[i].get_right_down_corner(), shift_up, tmp_name, font, pointerCOLOR, pointerCOLOR);
                brace_line.push_back(brace);
                arrow_start_line.push_back({(block_line[tmp_start].get_left_down_corner().x + block_line[i].get_right_down_corner().x) * 0.5f, block_line[0].get_left_down_corner().y + block_line[0].get_sideSIZE()[1] * 1.25f});
                arrow_end_line.push_back({(block_line[tmp_addr - base_addr].get_left_down_corner().x + block_line[tmp_addr - base_addr].get_right_down_corner().x) * 0.5f, block_line[0].get_left_down_corner().y});
                is_pointer_line.push_back(true);
            }
            else
            {
                Horizontal_brace brace(block_line[tmp_start].get_left_down_corner(), block_line[i].get_right_down_corner(), shift_up, tmp_name, font, varCOLOR, varCOLOR);
                brace_line.push_back(brace);
                arrow_start_line.push_back({0.f, 0.f});
                arrow_end_line.push_back({0.f, 0.f});
                is_pointer_line.push_back(false);
            }
        }
        tmp_name.clear();
        tmp_hex_addr.clear();
        tmp_is_pointer = false;
        tmp_start = 0;
        tmp_addr = 0;
    }
    float bias = 0.f;
    for (int k = 0; k < brace_line.size(); k++)
    {
        if (is_pointer_line[k])
        {
            bias += 1.f;
            Slip_Arrow arrow(arrow_start_line[k], arrow_end_line[k], bias * 50.f, block_line[0].get_sideSIZE()[1] * 0.01f, arrowCOLOR);
            arrow_line.push_back(arrow);
        }
    }
    this->brace_line = brace_line;
    this->arrow_line = arrow_line;
}


void InfoLine::draw(sf::RenderWindow & window)
{
    for (Horizontal_brace brace : this->brace_line)
        brace.draw(window);
    for (Slip_Arrow arrow : this->arrow_line)
        arrow.draw(window);
}


std::vector<Slip_Arrow> InfoLine::get_arrow_line()
{
    return this->arrow_line;
}


std::vector<Horizontal_brace> InfoLine::get_brace_line()
{
    return this->brace_line;
}