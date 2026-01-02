cmake_minimum_required(VERSION 3.10)
project(FridaDetector)
add_library(FridaDetector SHARED src/FridaDetector.cpp)
target_link_libraries(FridaDetector log)