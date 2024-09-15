cmake_minimum_required(VERSION 3.17)
set(PROJECT_NAME CH341CMD)

project(${PROJECT_NAME})

# Встановлюємо стандарт C++
set(CMAKE_CXX_STANDARD 17)

# Вказуємо шлях до заголовкових файлів CH341DLL
include_directories(${CMAKE_SOURCE_DIR})

# Додаємо виконуваний файл
add_executable(${PROJECT_NAME} main.cpp)

# Додаємо піддиректорію з cxxopts
add_subdirectory(extern/cxxopts)

# Вказуємо заголовкові файли для cxxopts
target_include_directories(${PROJECT_NAME} PUBLIC cxxopts)

# Лінкуємо cxxopts до проекту
target_link_libraries(${PROJECT_NAME} PUBLIC cxxopts)

# Вказуємо шлях до бібліотеки CH341DLL.lib
link_directories(${CMAKE_SOURCE_DIR})

# Лінкуємо з бібліотекою CH341DLL
target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/CH341DLL.lib)


# Для Windows додаємо необхідні системні бібліотеки
if (WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC setupapi)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -static-libgcc -static-libstdc++")
endif()
