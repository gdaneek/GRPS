#ifndef BRACES
#define BRACES



#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>



struct Horizontal_brace
{
public:
    Horizontal_brace();
    Horizontal_brace(sf::Vector2f start, sf::Vector2f end, float shift_up, std::string str, sf::Font font, sf::Color braceCOLOR = sf::Color::Black, sf::Color strCOLOR = sf::Color::Black);
    void draw(sf::RenderWindow & window);
private:
    sf::VertexArray brace;
    float shift_up;
    std::string str;
    sf::Color strCOLOR;
    sf::Font font;
};



#endif