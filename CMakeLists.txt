
cmake_minimum_required (VERSION 3.20)

project("hibiscus" VERSION 0.0.1 DESCRIPTION "Html parser" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(package.cmake)
include(config.cmake)

message(STATUS "Package libraries: ${PACKAGE_LIBRARIES}")

file(GLOB HIBISCUS_SRC_FILES "source/*.hpp" "source/*.cpp")

add_executable(hibiscus ${HIBISCUS_SRC_FILES})

#add_subdirectory("tests")

target_include_directories(${PROJECT_NAME} PUBLIC 
    "${CMAKE_SOURCE_DIR}/source"
    "${CMAKE_SOURCE_DIR}/ThirdParty/include"
)

target_link_directories(${PROJECT_NAME} PUBLIC 
    "${CMAKE_SOURCE_DIR}/ThirdParty/libs/yaml-cpp"
)

target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt) 

