cmake_minimum_required(VERSION 3.10)
project(web_utils)
add_library(web_utils SHARED src/FridaDetector.cpp)
target_link_libraries(web_utils log)
set_target_properties(web_utils PROPERTIES 
    OUTPUT_NAME "web_utils"
)