#ifndef ARROWS
#define ARROWS



#include <SFML/Graphics.hpp>
#include <cmath>



struct Arrow
{
public:
    Arrow();
    Arrow(const sf::Vector2f& start,
          const sf::Vector2f& end,
          float head_length = 15.f,
          float head_width = 10.f,
          sf::Color color = sf::Color::White
         );
    void draw(sf::RenderWindow & window);
private:
    sf::VertexArray line;
    sf::VertexArray head;
};


struct Slip_Arrow
{
public:
    Slip_Arrow();
    Slip_Arrow(sf::Vector2f start, sf::Vector2f end, float slip, float scale, sf::Color color = sf::Color::Black);
    void draw(sf::RenderWindow & window);
private:
    sf::VertexArray slip_line;
    sf::VertexArray base_line;
    Arrow arrow_line;
};



#endif