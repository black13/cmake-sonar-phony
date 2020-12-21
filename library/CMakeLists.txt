
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core network REQUIRED)

include_directories( ../include)
set(SOURCE_FILES logger.cpp 
masterHandshake.cpp	
pingMsg.cpp
sonarHandshakeMsg.cpp
masterCommand.cpp
message.cpp
sonarConnection.cpp
sonarMsg.cpp)

add_library(sonar STATIC ${SOURCE_FILES})
target_link_libraries(sonar Qt5::Core Qt5:Network)