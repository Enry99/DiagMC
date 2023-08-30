cmake_minimum_required(VERSION 3.16)

project("DiagMC" VERSION 1.0)


#Set compilation options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
string(APPEND CMAKE_CXX_FLAGS " -Wall")


#Enable use of CTest
include(CTest)


#Add library with functions
add_library(functions src/functions.cpp)
target_include_directories(functions PUBLIC include)

#Add main program executable
add_executable(2level.out src/main.cpp)
target_link_libraries(2level.out functions)

#Add tests
if (BUILD_TESTING)
add_subdirectory(tests)
endif()

#Set folder of the final executable
set(INSTALL_DIR "${CMAKE_SOURCE_DIR}/bin")
install(TARGETS 2level.out DESTINATION ${INSTALL_DIR})
