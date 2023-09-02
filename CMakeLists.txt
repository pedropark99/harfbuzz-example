cmake_minimum_required(VERSION 3.1)
project(harfbuzz-rust-example VERSION 1.0 LANGUAGES C CXX)


find_package(freetype)
find_package(harfbuzz)


add_executable(harfbuzz-c 
	src/main.c 
)


target_link_libraries(harfbuzz-c gd freetype harfbuzz m)


