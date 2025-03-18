cmake_minimum_required(VERSION 3.14)
project(ERP)

# Включаем модуль FetchContent
include(FetchContent)

# Настраиваем загрузку SFML 3.0.0
FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0  # Убедитесь, что тег 3.0.0 существует в репозитории
)

# Загружаем и делаем SFML доступным
FetchContent_MakeAvailable(sfml)

# Создаем исполняемый файл
add_executable(${PROJECT_NAME} main.cpp src/arrows.cpp src/braces.cpp src/blocks.cpp src/lines.cpp src/memory_vizual)

# Создаем исполняемый файл тестов
add_executable(test_ERP main_tests.cpp tests/tests.cpp src/arrows.cpp src/braces.cpp src/blocks.cpp src/lines.cpp src/memory_vizual)

# Устанавливаем стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Связываем SFML библиотеки с нашим проектом
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)

# Связываем SFML библиотеки с тестом с нашим проектом
target_link_libraries(test_ERP
    PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)

# Добавляем автоматическую загрузку зависимостей для Windows (если нужно)
if(WIN32)
    # Копируем DLL файлы SFML в выходную директорию
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:sfml-graphics>
        $<TARGET_FILE:sfml-window>
        $<TARGET_FILE:sfml-system>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()


# cmake_minimum_required(VERSION 3.10)
# project(ERP)

# include_directories(C:/Users/korve/Downloads/SFML-3.0.0/include)

# add_executable(MyApp main.cpp src/funcs.cpp src/arrows.cpp src/braces.cpp src/blocks.cpp src/lines.cpp src/memory_vizual)

# # Линковка SFML
# target_link_libraries(MyApp PUBLIC 
# C:/Users/korve/Downloads/SFML-3.0.0/lib/libsfml-graphics.a
# C:/Users/korve/Downloads/SFML-3.0.0/lib/libsfml-system.a 
# C:/Users/korve/Downloads/SFML-3.0.0/lib/libsfml-window.a)