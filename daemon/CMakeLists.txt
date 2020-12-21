project(daemon)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


add_definitions(-DSONARPHONY_VERSION="1")
add_definitions(-DSONARPHONY_COPYRIGHT_YEARS="2021")
add_definitions(-DSONARPHONY_AUTHOR="TBD")
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)

include_directories( ../include)
set(SOURCE_FILES daemon.cpp main.cpp)
add_executable(daemon ${SOURCE_FILES})
target_link_libraries(daemon Qt5::Core Qt5::Network sonar)