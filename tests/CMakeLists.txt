
#Download GoogleTest################################
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
####################################################


#add test executable
add_executable(test.out tests.cpp)
target_link_libraries(test.out gtest_main diagram)

#add_test(prova_gtests, test.out)
include(GoogleTest)
gtest_discover_tests(test.out)
