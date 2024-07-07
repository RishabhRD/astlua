#pragma once

#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace lua::parser::ast {
namespace __ast_details {
template <typename T>
bool is_equal(std::unique_ptr<T> const& a, std::unique_ptr<T> const& b) {
  if (a == nullptr && b == nullptr)
    return true;
  if (a != nullptr && b != nullptr)
    return *(a.get()) == *(b.get());
  return false;
}
}  // namespace __ast_details

// Forward declarations
struct statement;
struct prefix_expr;
struct expr;

// Handle types
using prefix_handle = std::unique_ptr<prefix_expr>;
using expr_handle = std::unique_ptr<expr>;

// Basic data structures
struct block {
  std::vector<statement> stats;

  friend bool operator==(block const&, block const&) = default;
};

enum class unary_op {
  MINUS,
  NOT,
  HASH,
};

enum class binary_op {
  PLUS,
  MINUS,
  PROD,
  DIV,
  EXP,
  MOD,
  CONCAT,
  LT,
  LTE,
  GT,
  GTE,
  EQ,
  NE,
  AND,
  OR,
};

template <typename T>
struct list_1 {
  T first;
  std::vector<T> more;
  friend bool operator==(list_1 const&, list_1 const&) = default;
};

// Variants and Expressions
struct expression_var {
  prefix_handle field;
  expr_handle property;

  friend bool operator==(expression_var const& a, expression_var const& b) {
    return __ast_details::is_equal(a.field, b.field) &&
           __ast_details::is_equal(a.property, b.property);
  }
};

struct name_var {
  prefix_handle field;
  std::string property;

  friend bool operator==(name_var const& a, name_var const& b) {
    return __ast_details::is_equal(a.field, b.field) &&
           a.property == b.property;
  }
};

using var = std::variant<std::string, expression_var, name_var>;

enum class field_sep {
  COMMA,
  SEMICOLON,
};

struct expr_field {
  expr_handle key;
  expr_handle value;

  friend bool operator==(expr_field const& a, expr_field const& b) {
    return __ast_details::is_equal(a.key, b.key) &&
           __ast_details::is_equal(a.value, b.value);
  }
};

struct name_field {
  std::string key;
  expr_handle value;

  friend bool operator==(name_field const& a, name_field const& b) {
    return a.key == b.key && __ast_details::is_equal(a.value, b.value);
  }
};

struct value_field {
  expr_handle value;

  friend bool operator==(value_field const& a, value_field const& b) {
    return __ast_details::is_equal(a.value, b.value);
  }
};

using field = std::variant<expr_field, name_field, value_field>;

struct table {
  std::vector<field> fields;

  friend bool operator==(table const&, table const&) = default;
};

struct string {
  std::string value;

  friend bool operator==(string const&, string const&) = default;
};

using args = std::variant<std::vector<expr>, table, string>;

struct fn_call {
  prefix_handle prefix;
  std::optional<std::string> field_name;

  friend bool operator==(fn_call const& a, fn_call const& b) {
    return __ast_details::is_equal(a.prefix, b.prefix) &&
           a.field_name == b.field_name;
  }
};

struct nil {
  friend bool operator==(nil, nil) = default;
};

struct false_t {
  friend bool operator==(false_t, false_t) = default;
};

struct true_t {
  friend bool operator==(true_t, true_t) = default;
};

struct number {
  std::string value;
  friend bool operator==(number, number) = default;
};

struct vararg {
  friend bool operator==(vararg, vararg) = default;
};

struct fn_body {
  std::vector<std::string> params;
  bool has_vararg;
  block fn_block;

  friend bool operator==(fn_body const&, fn_body const&) = default;
};

struct fn {
  fn_body body;
  friend bool operator==(fn const&, fn const&) = default;
};

struct binary_expr {
  binary_op op;
  expr_handle left_expr;
  expr_handle right_expr;

  friend bool operator==(binary_expr const& a, binary_expr const& b) {
    return a.op == b.op && __ast_details::is_equal(a.left_expr, b.left_expr) &&
           __ast_details::is_equal(a.right_expr, b.right_expr);
  }
};

struct unary_expr {
  unary_op op;
  expr_handle expr;

  friend bool operator==(unary_expr const& a, unary_expr const& b) {
    return a.op == b.op && __ast_details::is_equal(a.expr, b.expr);
  }
};

struct prefix_expr : public std::variant<var, fn_call, expr_handle> {
  using std::variant<var, fn_call, expr_handle>::variant;
};

struct expr : public std::variant<nil, false_t, true_t, number, fn, prefix_expr,
                                  table, binary_expr, unary_expr, vararg> {
  using std::variant<nil, false_t, true_t, number, fn, prefix_expr, table,
                     binary_expr, unary_expr, vararg>::variant;
};

// Statements
struct var_assign_stat {
  list_1<var> vars;
  list_1<expr> exprs;

  friend bool operator==(var_assign_stat const&,
                         var_assign_stat const&) = default;
};

struct do_stat {
  block body;
  friend bool operator==(do_stat const&, do_stat const&) = default;
};

struct while_stat {
  expr condition;
  block body;

  friend bool operator==(while_stat const&, while_stat const&) = default;
};

struct repeat_stat {
  expr condition;
  block body;

  friend bool operator==(repeat_stat const&, repeat_stat const&) = default;
};

struct elseif_code {
  expr condition;
  block body;

  friend bool operator==(elseif_code const&, elseif_code const&) = default;
};

struct else_code {
  block body;
  friend bool operator==(else_code const&, else_code const&) = default;
};

struct if_stat {
  expr condition;
  block body;
  std::vector<elseif_code> else_if_part;
  std::optional<else_code> else_part;
  friend bool operator==(if_stat const&, if_stat const&) = default;
};

struct for_range_stat {
  std::string loop_var;
  expr range_start;
  expr range_end;
  std::optional<expr> step;
  block body;

  friend bool operator==(for_range_stat const&,
                         for_range_stat const&) = default;
};

struct for_in_stat {
  list_1<std::string> vars;
  list_1<expr> exprs;
  friend bool operator==(for_in_stat const&, for_in_stat const&) = default;
};

struct fn_decl_stat {
  std::string name;
  fn_body fn;
  friend bool operator==(fn_decl_stat const&, fn_decl_stat const&) = default;
};

struct local_fn_decl_stat {
  std::string name;
  fn_body fn;
  friend bool operator==(local_fn_decl_stat const&,
                         local_fn_decl_stat const&) = default;
};

struct var_decl_stat {
  list_1<std::string> names;
  std::optional<list_1<expr>> exprs;
  friend bool operator==(var_decl_stat const&, var_decl_stat const&) = default;
};

struct statement
    : public std::variant<var_assign_stat, do_stat, while_stat, repeat_stat,
                          if_stat, for_range_stat, for_in_stat, fn_decl_stat,
                          local_fn_decl_stat, var_decl_stat> {
  using std::variant<var_assign_stat, do_stat, while_stat, repeat_stat, if_stat,
                     for_range_stat, for_in_stat, fn_decl_stat,
                     local_fn_decl_stat, var_decl_stat>::variant;
};

// Root of AST
using ast = block;
}  // namespace lua::parser::ast
