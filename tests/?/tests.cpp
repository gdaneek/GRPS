#include "tests.hpp"



void test_func_1()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML window");
 
    // Load a sprite to display
    const sf::Texture texture("../resources/img/green_barrel.jpg");
    sf::Sprite sprite(texture);
 
    // Create a graphical text to display
    const sf::Font font("../resources/fonts/HSESans-Black.otf");
    sf::Text text(font, "Hello SFML", 50);
 
    // Load a music to play
    // sf::Music music("resources/sounds/Till_I_Collapse.ogg");
 
    // Play the music
    // music.play();
 
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
        }
 
        // Clear screen
        window.clear();
 
        // Draw the sprite
        window.draw(sprite);
 
        // Draw the string
        window.draw(text);
 
        // Update the window
        window.display();
    }
}


void test_arrow_2()
{
    sf::RenderWindow window(sf::VideoMode({1000, 1000}), "SFML Arrow 2 Example");
    
    Slip_Arrow tst_1({300, 300}, {800, 200}, 150, 1, sf::Color::Black);
    Slip_Arrow tst_2({400, 400}, {700, 400}, 250, 1, sf::Color::Blue);

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        window.clear(sf::Color::White);
        tst_1.draw(window);
        tst_2.draw(window);
        window.display();
    }
}


void test_braces_1()
{
    sf::RenderWindow window(sf::VideoMode({1000, 1000}), "SFML Brace 1 Example");

    const sf::Font font("../resources/fonts/arial.ttf");

    Horizontal_brace tst_1({100, 300}, {700, 300}, -200.f, "x", font);

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();

            window.clear(sf::Color::White);
            tst_1.draw(window);
            window.display();
        }
    }
}



void test_block_1()
{
    sf::RenderWindow window(sf::VideoMode({1000, 1000}), "SFML Block 1 Example");

    const sf::Font font("../resources/fonts/arial.ttf");
    Block tst_1({100, 100}, 0.5, font, 86, 124);

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();

            window.clear(sf::Color::White);
            tst_1.draw(window);
            window.display();
        }
    }
}


void test_block_2()
{
    sf::RenderWindow window(sf::VideoMode({1000, 1000}), "SFML Block 2 Example");

    const sf::Font font("../resources/fonts/arial.ttf");
    std::vector<MemoryByte> memoryBYTES{{18, 79, "", false},
                                        {50, 172, "a", false},
                                        {141, 95, "", false},
                                        {194, 73, "b", false},
                                        {16, 236, "", false}};
    BlockLine tst_2(memoryBYTES, {200, 200}, 1, font, sf::Color::Green, sf::Color::Cyan, sf::Color::Blue, sf::Color::Red, sf::Color::Red, sf::Color::Red);

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();

            window.clear(sf::Color::White);
            tst_2.draw(window);
            window.display();
        }
    }
}



void test_view_1()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Horizontal Scroll");
    sf::View view = window.getDefaultView();
    
    // Параметры прокрутки
    sf::Vector2f offset(0.f, 0.f);
    float scrollSpeed = 20.f;
    sf::Vector2f contentSize(100.f, 50.f); // Размер контента
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

        // Фон
        // sf::RectangleShape background(contentSize);
        // background.setFillColor(sf::Color::White);
        // window.draw(background);

        const sf::Font font("../resources/fonts/arial.ttf");

        std::vector<MemoryByte> memoryBYTES_1{
                                {67, 0, "px", true},
                                {68, 0, "px", true},
                                {69, 0, "px", true},
                                {70, 71, "px", true},
                                {71, 0, "x", false},
                                {72, 0, "x", false},
                                {73, 7, "x", false},
                                {74, 233, "x", false},
                                {75, 167, "", false},
                                {76, 215, "", false},
                                {77, 0, "ppx", true},
                                {78, 0, "ppx", true},
                                {79, 0, "ppx", true},
                                {80, 67, "ppx", true},
                                };
        
        std::vector<MemoryByte> memoryBYTES_2{
                                    {0x01, 0xff, "a", false},
                                    {0x02, 0xff, "a", false},
                                    {0x03, 0xff, "a", false},
                                    {0x04, 0xff, "a", false },
                                    {0x05, 0xce, "", false},
                                    {0x06, 0xfe, "", false},
                                    {0x07, 0xcf, "", false},
                                    {0x08, 0xbc, "", false},
                                    {0x09, 0x00, "ptr", true},
                                    {0x0a, 0x00, "ptr", true},
                                    {0x0b, 0x00, "ptr", true},
                                    {0x0c, 0x01, "ptr", true},
                                    };
        
        std::vector<MemoryByte> memoryBYTES_3{
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

        BlockLine tst_line(memoryBYTES_3, {1000, 250}, scale, font);
        tst_line.draw(window);

        InfoLine tst2_line(memoryBYTES_3, tst_line.get_block_line(), font, sf::Color::Blue, sf::Color(160, 23, 194));
        tst2_line.draw(window);

        // std::vector<std::tuple<std::string, std::string, bool>> data{
        //     std::tuple<std::string, std::string, bool>{"12", "4F", false},
        //     std::tuple<std::string, std::string, bool>{"32", "AC", true},
        //     std::tuple<std::string, std::string, bool>{"8D", "5F", false},
        //     std::tuple<std::string, std::string, bool>{"C2", "49", true},
        //     std::tuple<std::string, std::string, bool>{"10", "EC", false}};
        // BlockLine tst_line(data, {3500, 250}, 100, font, sf::Color::Black, sf::Color::Black, sf::Color::Black, sf::Color::Red, sf::Color::Red, sf::Color::Red);
        // tst_line.draw(window);

        // 11068046444225730969
        // Block b0({3500, 250}, 1, font, 135790, 233, sf::Color::Red, sf::Color::Cyan, sf::Color::Green);
        // b0.draw(window);

        // Доп точка
        // sf::VertexArray dop(sf::PrimitiveType::Points, 1);
        // dop[0].position = {0, 0};
        // dop[0].color = sf::Color::White;
        // window.draw(dop);

        // Update the window
        window.display();
    }
}


void test_INTtoHEX()
{
    // 2712847316
    // 2748
    MemoryByte b0{2748, 0, "a", true};
    std::cout << UINT32toHEX(b0.addr) << std::endl;
    std::cout << UINT8toHEX(b0.value) << std::endl;
    system("pause");
}


void test_HEX8toUINT32()
{
    std::cout << HEX8toUINT32("ffffffff") << std::endl;
    system("pause");
}


void test_memory_vizual()
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
    std::vector<std::vector<int>> colorVEC{
                                            {201, 44, 112},
                                            {18, 115, 110},
                                            {135, 78, 173},
                                            {166, 51, 76},
                                            {5, 57, 245},
                                            {95, 212, 91},
                                            {163, 157, 31},
                                            {138, 93, 67},
                                            {5, 5, 20}
                                          };
    sf::Font font("../resources/fonts/arial.ttf");

    memoryVIZUALIZATION(memoryBYTES, font);
}