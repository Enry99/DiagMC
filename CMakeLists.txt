cmake_minimum_required(VERSION 3.16)

project("DiagMC" VERSION 0.1)

#Set compilation options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
string(APPEND CMAKE_CXX_FLAGS " -Wall")



#Enable use of CTest
include(CTest)


include(FetchContent)
FetchContent_Declare(
    json
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
    )
FetchContent_MakeAvailable(json)




#Add libraries with classes and functions
add_library(diagram src/diagram.cpp)
target_include_directories(diagram PUBLIC include)

add_library(simulation src/simulation.cpp)
target_include_directories(simulation PUBLIC include)

add_library(setup src/setup.cpp)
target_include_directories(setup PUBLIC include)
target_link_libraries(setup PUBLIC nlohmann_json::nlohmann_json diagram simulation)


#Add main program executable
add_executable(2levelDiagMC src/main.cpp)
target_link_libraries(2levelDiagMC simulation diagram setup)

#Add tests
if (BUILD_TESTING)
set(INSTALL_GTEST  OFF)
add_subdirectory(tests)
endif()

#Set folder of the final executable
set(INSTALL_DIR "${CMAKE_SOURCE_DIR}/bin")
install(TARGETS 2levelDiagMC DESTINATION ${INSTALL_DIR})
