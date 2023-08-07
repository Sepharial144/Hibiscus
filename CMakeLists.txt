
cmake_minimum_required (VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project("hibiscus" VERSION 0.0.1 DESCRIPTION "Html parser" LANGUAGES CXX)

file(GLOB SRC_FILES "source/*.hpp" "source/*.cpp")

add_executable(hibiscus ${SRC_FILES})

enable_testing()
add_subdirectory("tests")

target_include_directories(${PROJECT_NAME} PUBLIC 
    "${CMAKE_SOURCE_DIR}/ThirdParty/include"
)

target_link_directories(${PROJECT_NAME} PUBLIC 
    "${CMAKE_SOURCE_DIR}/ThirdParty/libs/yaml-cpp"
)

