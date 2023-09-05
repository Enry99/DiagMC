
#Download GoogleTest################################
include(FetchContent)
FetchContent_Declare(
  googletest
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
####################################################


#add test executable
add_executable(tests tests.cpp)
target_link_libraries(tests gtest_main diagram simulation)


include(GoogleTest)
gtest_discover_tests(tests)


#automatically runs test after building
#add_custom_command(
#     TARGET tests
#     COMMENT "Run tests"
#     POST_BUILD 
#     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#     COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failures
#)
