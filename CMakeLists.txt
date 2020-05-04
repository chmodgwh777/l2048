cmake_minimum_required(VERSION 3.5)

project(l2048 LANGUAGES CXX)

#set(CMAKE_PREFIX_PATH /Users/gao/Qt5.9.9/5.9.9/clang_64)
#set(QT_QMAKE_EXECUTABLE /Users/gao/Qt5.9.9/5.9.9/clang_64/bin/qmake)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SOURCES
    src/main.cpp
    src/block.cpp
    src/mainwindow.cpp
)

set(UI_FORMS
    ui/mainwindow.ui
)

add_executable(learn
    ${SOURCES}
    ${UI_FORMS}
)

target_link_libraries(learn 
    PRIVATE 
    Qt5::Widgets
)

target_include_directories(learn
    PRIVATE 
    ${PROJECT_SOURCE_DIR}/include
)
