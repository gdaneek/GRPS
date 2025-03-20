#include "../include/arrows.hpp"



Arrow::Arrow(){}


Arrow::Arrow(const sf::Vector2f &start, const sf::Vector2f &end, float head_length, float head_width, sf::Color color)
{
    // Создаем основную линию стрелки
    line = sf::VertexArray(sf::PrimitiveType::Lines, 2);
    line[0].position = start;
    line[1].position = end;
    line[0].color = color;
    line[1].color = color;

    // Рассчитываем направление стрелки
    const sf::Vector2f direction = end - start;
    const float length = std::hypot(direction.x, direction.y);
    
    // Если точки совпадают, возвращаем пустую стрелку
    if (!(length < std::numeric_limits<float>::epsilon()))
    {
        // Нормализуем вектор направления
        const sf::Vector2f unit_direction = direction / length;

        // Рассчитываем точки наконечника
        const sf::Vector2f perpendicular(-unit_direction.y, unit_direction.x);
        const sf::Vector2f back_offset = unit_direction * (-head_length);
        const sf::Vector2f side_offset = perpendicular * (head_width / 2.f);

        // Создаем треугольник наконечника
        head = sf::VertexArray(sf::PrimitiveType::Triangles, 3);
        head[0].position = end + back_offset + side_offset;
        head[1].position = end + back_offset - side_offset;
        head[2].position = end;

        // Устанавливаем цвет наконечника
        head[0].color = color;
        head[1].color = color;
        head[2].color = color;

        this->head = head;
    }
    this->line = line;
}


void Arrow::draw(sf::RenderWindow & window)
{
    window.draw(this->line);
    window.draw(this->head);
}


Slip_Arrow::Slip_Arrow(){}


Slip_Arrow::Slip_Arrow(sf::Vector2f start, sf::Vector2f end, float slip, float scale, sf::Color color)
{
    // Make dop-points
    sf::Vector2f dop1(start.x, start.y + slip);
    sf::Vector2f dop2(end.x, dop1.y);

    // Make slip-line
    sf::VertexArray slip_line(sf::PrimitiveType::Lines, 2);
    slip_line[0].position = start;
    slip_line[1].position = dop1;
    slip_line[0].color = color;
    slip_line[1].color = color;

    // Make base-line
    sf::VertexArray base_line(sf::PrimitiveType::Lines, 2);
    base_line[0].position = dop1;
    base_line[1].position = dop2;
    base_line[0].color = color;
    base_line[1].color = color;

    // Make arrow-line
    Arrow arrow_line(dop2, end, 20.f * scale, 15.f * scale, color);

    // Fill the fields of structure
    this->slip_line = slip_line;
    this->base_line = base_line;
    this->arrow_line = arrow_line;
}


void Slip_Arrow::draw(sf::RenderWindow & window)
{
    window.draw(this->slip_line);
    window.draw(this->base_line);
    this->arrow_line.draw(window);
}