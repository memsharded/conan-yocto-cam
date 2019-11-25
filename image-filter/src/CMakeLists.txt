project(imagefilter VERSION 0.1 LANGUAGES CXX)

add_library(imagefilter Decoder.cpp
                        Detector.cpp
                        Receiver.cpp)
target_include_directories(imagefilter PUBLIC .)

add_executable(app  main.cpp)
target_link_libraries(app imagefilter ${CONAN_LIBS})
