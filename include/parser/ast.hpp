#pragma once

#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace lua {
namespace ast {

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
};

struct name_var {
  prefix_handle field;
  std::string property;
};

using var = std::variant<std::string, expression_var, name_var>;

enum class field_sep {
  COMMA,
  SEMICOLON,
};

struct expr_field {
  expr_handle key;
  expr_handle value;
};

struct name_field {
  std::string key;
  expr_handle value;
};

struct value_field {
  expr_handle value;
};

using field = std::variant<expr_field, name_field, value_field>;

struct table {
  std::vector<field> fields;
};

struct string {
  std::string value;
};

using args = std::variant<std::vector<expr>, table, string>;

struct fn_call {
  prefix_handle prefix;
  std::optional<std::string> field_name;
};

struct nil {};

struct false_t {};

struct true_t {};

struct number {
  std::string value;
};

struct vararg {};

struct fn_body {
  std::vector<std::string> params;
  bool has_vararg;
  block fn_block;
};

struct fn {
  fn_body body;
};

struct binary_expr {
  binary_op op;
  expr_handle left;
  expr_handle right;
};

struct unary_expr {
  unary_op op;
  expr_handle expr;
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
};

struct do_stat {
  block body;
};

struct while_stat {
  expr condition;
  block body;
};

struct repeat_stat {
  expr condition;
  block body;
};

struct elseif_code {
  expr condition;
  block body;
};

struct else_code {
  block body;
};

struct if_stat {
  expr condition;
  block body;
  std::vector<elseif_code> else_if_part;
  std::optional<else_code> else_part;
};

struct for_range_stat {
  std::string loop_var;
  expr range_start;
  expr range_end;
  std::optional<expr> step;
  block body;
};

struct for_in_stat {
  list_1<std::string> vars;
  list_1<expr> exprs;
};

struct fn_decl_stat {
  std::string name;
  fn_body fn;
};

struct local_fn_decl_stat {
  std::string name;
  fn_body fn;
};

struct var_decl_stat {
  list_1<std::string> names;
  std::optional<list_1<expr>> exprs;
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

}  // namespace ast
}  // namespace lua
