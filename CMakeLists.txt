cmake_minimum_required(VERSION 3.17)
project(CH341CMD)

# Встановлюємо стандарт C++
set(CMAKE_CXX_STANDARD 17)

# Вказуємо шлях до заголовкових файлів CH341DLL
include_directories(${CMAKE_SOURCE_DIR})

# Вказуємо шлях до бібліотеки CH341DLL
link_directories(${CMAKE_SOURCE_DIR})

# Додаємо виконуваний файл
add_executable(CH341CMD main.cpp)

# Лінкуємо з бібліотекою CH341DLL
target_link_libraries(CH341CMD CH341DLL)

# Для Windows додаємо необхідні системні бібліотеки
if (WIN32)
    target_link_libraries(CH341CMD setupapi)
endif()
