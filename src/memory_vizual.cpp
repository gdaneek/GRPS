#include "../include/memory_vizual.hpp"



void memoryVIZUALIZATION(std::vector<MemoryByte> memoryBYTES, sf::Font font, sf::Vector2f start, std::vector<std::vector<int>> colorVEC)
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Memory Vizualization");
    sf::View view = window.getDefaultView();
    
    // Параметры прокрутки
    sf::Vector2f offset(0.f, 0.f);
    float scrollSpeed = 20.f;
    sf::Vector2f contentSize(memoryBYTES.size() * 350.f + (start.x + 1) * 2.f, memoryBYTES.size() * 12.5f + (start.y + 1) * 2.f); // Размер контента
    sf::Vector2f viewSize = view.getSize();
    sf::Vector2f maxOffset;
    maxOffset.x = std::max(contentSize.x - viewSize.x, 0.f);
    maxOffset.y = std::max(contentSize.y - viewSize.y, 0.f);
    
    // Для перетаскивания мышью
    bool dragging = false;
    sf::Vector2i dragStartPosition;
    sf::Vector2f dragStartOffset;
    bool check_ctrl = false;

    float scale = 1.f;
    sf::Time sleepTIME = sf::milliseconds(16.67f);

    // Start the game loop
    while (window.isOpen())
    {
        sf::sleep(sleepTIME);
        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Закрытие окна
            if (event->is<sf::Event::Closed>())
                window.close();

            // Изменениу размера окна
            if (const auto* resized = event->getIf<sf::Event::Resized>())
            {
                view.setSize({resized->size.x, resized->size.y});
                viewSize = view.getSize();
                maxOffset.x = std::max(contentSize.x - viewSize.x, 0.f);
                maxOffset.y = std::max(contentSize.y - viewSize.y, 0.f);
                offset.x = std::clamp(offset.x, 0.f, maxOffset.x);
                offset.y = std::clamp(offset.y, 0.f, maxOffset.y);
                view.setCenter(offset + viewSize * 0.5f);
                window.setView(view);
            }
    
            // Прокрутка колесиком мыши
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scan::LControl || keyPressed->scancode == sf::Keyboard::Scan::RControl)
                {
                    check_ctrl ^= true;
                }
            }
            if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
            {
                if (mouseWheelScrolled->wheel == sf::Mouse::Wheel::Vertical)
                {
                    if (check_ctrl)
                    {
                        offset.y -= mouseWheelScrolled->delta * scrollSpeed;
                    }
                    else
                    {
                        offset.x -= mouseWheelScrolled->delta * scrollSpeed;
                    }
                    offset.x = std::clamp(offset.x, 0.f, maxOffset.x);
                    offset.y = std::clamp(offset.y, 0.f, maxOffset.y);
                    view.setCenter(offset + viewSize * 0.5f);
                    window.setView(view);
                }
            }
            
            // Перетаскивание вида мышью
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left)
                {
                    dragging = true;
                    dragStartPosition = sf::Mouse::getPosition(window);
                    dragStartOffset = offset;
                }
            }
            if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
            {
                if (mouseButtonReleased->button == sf::Mouse::Button::Left)
                {
                    dragging = false;
                }
            }
            if (event->is<sf::Event::MouseMoved>() && dragging)
            {
                sf::Vector2i currentPos = sf::Mouse::getPosition(window);
                sf::Vector2f delta = sf::Vector2f(
                    dragStartPosition.x - currentPos.x,
                    dragStartPosition.y - currentPos.y
                );
                offset = dragStartOffset + delta;
                offset.x = std::clamp(offset.x, 0.f, maxOffset.x);
                offset.y = std::clamp(offset.y, 0.f, maxOffset.y);
                view.setCenter(offset + viewSize * 0.5f);
                window.setView(view);
            }

            // Прокрутка вида стрелками
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scan::Left)
                {
                    offset.x -= scrollSpeed;
                }
                if (keyPressed->scancode == sf::Keyboard::Scan::Right)
                {
                    offset.x += scrollSpeed;
                }
                if (keyPressed->scancode == sf::Keyboard::Scan::Up)
                {
                    offset.y -= scrollSpeed;
                }
                if (keyPressed->scancode == sf::Keyboard::Scan::Down)
                {
                    offset.y += scrollSpeed;
                }
                offset.x = std::clamp(offset.x, 0.f, maxOffset.x);
                offset.y = std::clamp(offset.y, 0.f, maxOffset.y);
                view.setCenter(offset + viewSize * 0.5f);
                window.setView(view);
            }

            // Изменение маштаба отрисовки структуры памятит
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scan::LAlt)
                {
                    scale -= 0.05f;
                }
                if (keyPressed->scancode == sf::Keyboard::Scan::RAlt)
                {
                    scale += 0.05f;
                }
            }
        }

        // Отрисовка
        window.clear(sf::Color::White);

        if (colorVEC.empty())
        {
            BlockLine block_line(memoryBYTES, start, scale, font);
            InfoLine info_line(memoryBYTES, block_line.get_block_line(), font);
            block_line.draw(window);
            info_line.draw(window);
        }
        else
        {
            BlockLine block_line(memoryBYTES, start, scale, font, sf::Color(colorVEC[0][0], colorVEC[0][1], colorVEC[0][2]), sf::Color(colorVEC[1][0], colorVEC[1][1], colorVEC[1][2]), sf::Color(colorVEC[2][0], colorVEC[2][1], colorVEC[2][2]), sf::Color(colorVEC[3][0], colorVEC[3][1], colorVEC[3][2]), sf::Color(colorVEC[4][0], colorVEC[4][1], colorVEC[4][2]), sf::Color(colorVEC[5][0], colorVEC[5][1], colorVEC[5][2]));
            InfoLine info_line(memoryBYTES, block_line.get_block_line(), font, sf::Color(colorVEC[6][0], colorVEC[6][1], colorVEC[6][2]), sf::Color(colorVEC[7][0], colorVEC[7][1], colorVEC[7][2]), sf::Color(colorVEC[8][0], colorVEC[8][1], colorVEC[8][2]));
            block_line.draw(window);
            info_line.draw(window);
        }

        // Update the window
        window.display();
    }
}