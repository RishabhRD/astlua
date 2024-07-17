file(GLOB_RECURSE src_list "../src/astlua/*.cpp")
add_executable(astlua ${src_list} main.cpp)
target_include_directories(astlua SYSTEM PRIVATE ../lib)
target_include_directories(astlua PRIVATE ../include)
target_link_libraries(astlua PRIVATE
  project_options
  project_warnings
)
