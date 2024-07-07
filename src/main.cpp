#include <iostream>
#include "parser/ast.hpp"

int main() {
  using namespace lua::parser;
  ast::ast a{};
  ast::ast b{};
  if (a == b) {}
  std::cout << a << std::endl;
}
