cmake_minimum_required(VERSION 3.5) # CMake install : https://cmake.org/download/
project(CS_Contest LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "D:/Qt/6.5.0/mingw_64") # Qt Kit Dir
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# find_package(Qt5 COMPONENTS Widgets REQUIRED) # Qt COMPONENTS
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets AxContainer)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets AxContainer)

aux_source_directory(./src srcs)
aux_source_directory(./component components)

list(APPEND CMAKE_AUTOUIC_SEARCH_PATHS "ui")

# Specify MSVC UTF-8 encoding   
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_executable(${PROJECT_NAME}
    WIN32 # If you need a terminal for debug, please comment this statement 
    ${srcs}
    ${components}
)

target_include_directories(${PROJECT_NAME} PRIVATE component)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets) # Qt5 Shared Library
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::AxContainer)