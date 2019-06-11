project(lib VERSION 0.1 LANGUAGES CXX)

add_library(lib src/Decoder.cpp
                src/Detector.cpp
                src/Receiver.cpp)
target_include_directories(lib PUBLIC include)
