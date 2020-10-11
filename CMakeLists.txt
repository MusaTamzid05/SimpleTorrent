cmake_minimum_required(VERSION 2.6)

project(testas)


set(Source
    src/main.cpp
    src/torrent.cpp)



include_directories(include )
add_executable(testas ${Source})


target_link_libraries(testas torrent-rasterbar boost_system boost_filesystem)
