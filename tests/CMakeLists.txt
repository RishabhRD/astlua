file(GLOB_RECURSE test_list "*_test.cpp")
file(GLOB_RECURSE src_list "../src/astlua/*.cpp")
add_executable(astlua-tests ${src_list} ${test_list} main.cpp)
target_include_directories(astlua-tests SYSTEM PRIVATE ../lib)
target_include_directories(astlua-tests PRIVATE ../include .)
target_link_libraries(astlua-tests PRIVATE
  project_options
  project_warnings
  doctest::doctest
)
