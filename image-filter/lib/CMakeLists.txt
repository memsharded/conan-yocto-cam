project(imagefilter VERSION 0.1 LANGUAGES CXX)

add_library(imagefilter src/Decoder.cpp
                src/Detector.cpp
                src/Receiver.cpp)
target_include_directories(imagefilter PUBLIC include)
