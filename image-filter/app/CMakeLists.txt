project(app VERSION 0.1 LANGUAGES CXX)

add_executable(app src/main.cpp)
target_link_libraries(app imagefilter ${CONAN_LIBS})