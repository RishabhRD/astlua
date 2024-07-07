#include "parser/ast.hpp"
#include <iterator>
#include <optional>
#include <variant>
#include "functional.hpp"

namespace lua::parser::ast {
template <typename T>
std::ostream& print(std::ostream& os, std::unique_ptr<T> const& x) {
  if (x == nullptr)
    os << "null";
  else
    os << *x;
  return os;
}

template <typename T>
std::ostream& print(std::ostream& os, std::optional<T> const& x) {
  if (x == std::nullopt)
    os << "null";
  else
    os << *x;
  return os;
}

template <typename T>
std::ostream& print(std::ostream& os, std::vector<T> const& vec) {
  os << "[ ";
  std::copy(vec.begin(), vec.end(), std::ostream_iterator<T>(os, " "));
  os << "]";
  return os;
}

std::ostream& print(std::ostream& os, table const& t) {
  os << "table{fields: ";
  print(os, t.fields);
  os << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, block const& b) {
  os << "block{";
  print(os, b.stats);
  os << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, unary_op const& op) {
  os << "unary_op{";
  switch (op) {
    case unary_op::MINUS:
      os << "-";
      break;
    case unary_op::NOT:
      os << "not";
      break;
    case unary_op::HASH:
      os << "#";
      break;
  }
  os << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, binary_op const& op) {
  os << "binary_op{";
  switch (op) {
    case binary_op::PLUS:
      os << "+";
      break;
    case binary_op::MINUS:
      os << "-";
      break;
    case binary_op::PROD:
      os << "*";
      break;
    case binary_op::DIV:
      os << "/";
      break;
    case binary_op::EXP:
      os << "^";
      break;
    case binary_op::MOD:
      os << "%";
      break;
    case binary_op::CONCAT:
      os << "..";
      break;
    case binary_op::LT:
      os << "<";
      break;
    case binary_op::LTE:
      os << "<=";
      break;
    case binary_op::GT:
      os << ">";
      break;
    case binary_op::GTE:
      os << ">=";
      break;
    case binary_op::EQ:
      os << "==";
      break;
    case binary_op::NE:
      os << "~=";
      break;
    case binary_op::AND:
      os << "and";
      break;
    case binary_op::OR:
      os << "or";
      break;
  }
  os << "}";
  return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, list_1<T> const& lst) {
  os << "[ ";
  os << lst.first << " ";
  std::copy(lst.more.begin(), lst.more.end(),
            std::ostream_iterator<T>(os, " "));
  os << "]";
  return os;
}

std::ostream& operator<<(std::ostream& os, expression_var const& x) {
  os << "expression_var{field: ";
  print(os, x.field);
  os << ", property: ";
  print(os, x.property);
  os << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, name_var const& x) {
  os << "name_var{field: ";
  print(os, x.field);
  os << ", property: " << x.property << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, var const& v_var) {
  std::visit([&os](auto const& v) { os << v; }, v_var);
  return os;
}

std::ostream& operator<<(std::ostream& os, field_sep const& x) {
  os << "field_sep{";
  switch (x) {
    case field_sep::COMMA:
      os << ",";
      break;
    case field_sep::SEMICOLON:
      os << ";";
  }
  os << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, expr_field const& x) {
  os << "expr_field{key: ";
  print(os, x.key);
  os << ", value: ";
  print(os, x.value);
  os << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, name_field const& x) {
  os << "expr_field{key: " << x.key << ", value: ";
  print(os, x.value);
  os << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, value_field const& x) {
  os << "value_field{value: ";
  print(os, x.value);
  os << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, field const& x) {
  std::visit([&os](auto const& a) { os << a; }, x);
  return os;
}

std::ostream& operator<<(std::ostream& os, table const& x) {
  os << "table{fields: ";
  print(os, x.fields);
  os << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, string const& x) {
  os << "string{value: " << x.value << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, args const& x) {
  os << "args{arguments:";
  std::visit(nostd::overload{
                 [&os](std::vector<expr> const& a) { print(os, a); },
                 [&os](table const& t) { os << t; },
                 [&os](string const& s) { os << s; },
             },
             x.arguments);
  return os;
}

std::ostream& operator<<(std::ostream& os, fn_call const& x) {
  os << "fn_call{prefix: ";
  print(os, x.prefix);
  os << ", field_name: ";
  print(os, x.field_name);
  os << "}";
  return os;
};

std::ostream& operator<<(std::ostream& os, nil const&) {
  os << "nil";
  return os;
};

std::ostream& operator<<(std::ostream& os, false_t const&) {
  os << "false";
  return os;
};

std::ostream& operator<<(std::ostream& os, true_t const&) {
  os << "true";
  return os;
};

std::ostream& operator<<(std::ostream& os, number const& x) {
  os << "number{" << x.value << "}";
  return os;
};

std::ostream& operator<<(std::ostream& os, vararg const&) {
  os << "...";
  return os;
}

std::ostream& operator<<(std::ostream& os, fn_body const& x) {
  os << "fn_body{params: ";
  print(os, x.params);
  os << " ,has_vararg: " << x.has_vararg << ", fn_block: " << x.fn_block << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, fn const& x) {
  os << "fn{body: " << x.body << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, binary_expr const& x) {
  os << "binary_expr{op: " << x.op << ", left_expr: ";
  print(os, x.left_expr);
  os << ", right_expr: ";
  print(os, x.right_expr);
  os << "}";
}

std::ostream& operator<<(std::ostream& os, unary_expr const& x) {
  os << "unary_expr{op: " << x.op << ", expr: ";
  print(os, x.expr);
  os << "}";
}

std::ostream& operator<<(std::ostream& os, prefix_expr const& x) {
  std::visit([&os](auto const& a) { os << a; }, x);
  return os;
}

std::ostream& operator<<(std::ostream& os, expr const& x) {
  std::visit([&os](auto const& a) { os << a; }, x);
  return os;
}

std::ostream& operator<<(std::ostream& os, var_assign_stat const& x) {
  os << "var_assign_stat{vars: " << x.vars << ", exprs: " << x.exprs << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, do_stat const& x) {
  os << "do_stat{body: " << x.body << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, while_stat const& x) {
  os << "while_stat{condition: " << x.condition << ", body: " << x.body << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, repeat_stat const& x) {
  os << "repeat_stat{condition: " << x.condition << ", body: " << x.body << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, elseif_code const& x) {
  os << "elseif{condition: " << x.condition << ", body: " << x.body << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, else_code const& x) {
  os << "else{body: " << x.body << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, if_stat const& x) {
  os << "if{condition: " << x.condition;
  os << ", body: " << x.body;
  os << ", else_if_part: ";
  print(os, x.else_if_part);
  os << ", else_part: ";
  print(os, x.else_part);
  os << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, for_range_stat const& x) {
  os << "for_range{loop_var: " << x.loop_var;
  os << ", range_start: " << x.range_start;
  os << ", range_end: " << x.range_end;
  os << ", step: ";
  print(os, x.step);
  os << ", body: " << x.body << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, for_in_stat const& x) {
  os << "for_in{vars: " << x.vars;
  os << ", exprs: " << x.exprs << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, fn_decl_stat const& x) {
  os << "fn_decl{name: " << x.name << ", fn: " << x.fn << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, local_fn_decl_stat const& x) {
  os << "local_fn_decl{name: " << x.name << ", fn: " << x.fn << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, var_decl_stat const& x) {
  os << "var_decl{names: " << x.names << ", exprs: ";
  print(os, x.exprs);
  os << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, statement const& x) {
  std::visit([&os](auto const& a) { os << a; }, x);
  return os;
}
}  // namespace lua::parser::ast
