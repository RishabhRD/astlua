#include <iostream>
#include "ast/ast.hpp"

int main() {
  using namespace lua::ast;
  ast a{};
  ast b{};
  if (a == b) {
    std::cout << "yes equal" << std::endl;
  }
  std::cout << a << std::endl;
}
