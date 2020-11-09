cmake_minimum_required(VERSION 3.15)
project(space)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DSDL_MAIN_HANDLED)

file(GLOB SRC "src/*.cpp" "src/*.h")

find_package(SDL2 CONFIG REQUIRED)

if (APPLE)
    include_directories(${SDL2_INCLUDE_DIRS})
endif()

add_executable(${PROJECT_NAME} ${SRC})
if (APPLE)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2_LIBRARIES})
else()
    target_link_libraries(${PROJECT_NAME} PUBLIC SDL2)
endif()