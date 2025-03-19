#include "../include/braces.hpp"



Horizontal_brace::Horizontal_brace(){}


Horizontal_brace::Horizontal_brace(sf::Vector2f start, sf::Vector2f end, float shift_up, std::string str, sf::Font font, sf::Color braceCOLOR, sf::Color strCOLOR)
{
    // Make points
    sf::Vector2f mid(((start.x + end.x)/2), start.y);
    sf::Vector2f pick(mid.x, mid.y + shift_up);
    sf::Vector2f base_start(start.x + abs(shift_up)/2, start.y + shift_up/2);
    sf::Vector2f base_end(end.x - abs(shift_up)/2, end.y + shift_up/2);
    sf::Vector2f pick_start(mid.x - abs(shift_up)/2, mid.y + shift_up/2);
    sf::Vector2f pick_end(mid.x + abs(shift_up)/2, mid.y + shift_up/2);

    // Make brace
    sf::VertexArray brace(sf::PrimitiveType::LineStrip, 7);

    // Make brace points
    brace[0].position = start;
    brace[1].position = base_start;
    brace[2].position = pick_start;
    brace[3].position = pick;
    brace[4].position = pick_end;
    brace[5].position = base_end;
    brace[6].position = end;

    // Make brace color
    for (int i = 0; i < 7; i++)
        brace[i].color = braceCOLOR;
    
    // Fill structure fields
    this->brace = brace;
    this->shift_up = shift_up;
    this->str = str;
    this->strCOLOR = strCOLOR;
    this->font = font;
}


void Horizontal_brace::draw(sf::RenderWindow & window)
{
    // Make text
    sf::Text text(this->font, this->str, std::abs(this->shift_up));
    text.setPosition({this->brace[3].position.x - (this->str.size() * std::abs(this->shift_up) * 0.25f), this->brace[3].position.y + (this->shift_up * 0.05f - (this->shift_up >= 0 ? 0.f : (std::abs(this->shift_up) * 0.5f)))});
    text.setFillColor(this->strCOLOR);

    window.draw(this->brace);
    window.draw(text);
}