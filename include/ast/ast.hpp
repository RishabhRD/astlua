#pragma once

#include <cstddef>
#include <memory>
#include <optional>
#include <ostream>
#include <string>
#include <variant>
#include <vector>

namespace lua::ast {

template <typename T>
class handle {
  std::unique_ptr<T> handle_;

 public:
  handle(std::unique_ptr<T> ptr_handle) : handle_(std::move(ptr_handle)) {}

  handle(T value) : handle_(std::make_unique<T>(std::move(value))) {}

  handle(handle&& other) : handle_(std::move(other.handle_)) {}

  handle(handle const& other)
      : handle_(other.handle_ ? std::make_unique<T>(*other.handle_) : nullptr) {
  }

  handle& operator=(handle other) {
    this->handle_ = std::move(other.handle_);
    return *this;
  }

  T& operator*() { return *handle_; }

  T& operator*() const { return *handle_; }

  bool operator==(std::nullptr_t) const { return handle_ == nullptr; }

  bool operator==(handle const& other) const {
    if (handle_ == nullptr && other.handle_ == nullptr)
      return true;
    if (handle_ != nullptr && other.handle_ != nullptr)
      return *handle_ == *(other.handle_);
    return false;
  }

  operator bool() const { return handle_ != nullptr; }
};

template <typename T>
handle(std::unique_ptr<T>) -> handle<T>;

template <typename T>
handle(T) -> handle<T>;

// Forward declarations
struct statement;
struct prefix_expr;
struct expr;

// Handle types
using prefix_handle = handle<prefix_expr>;
using expr_handle = handle<expr>;

struct return_stat {
  expr_handle expr;

  friend bool operator==(return_stat const&, return_stat const&) = default;
};

// Basic data structures
struct block {
  std::vector<statement> stats;
  std::optional<return_stat> return_statement;

  friend bool operator==(block const&, block const&) = default;
};

enum class unary_op {
  MINUS,
  NOT,
  LEN,
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

  list_1(T first_, std::vector<T> more_)
      : first(std::move(first_)), more(std::move(more_)) {}

  list_1(T first_) : first(std::move(first_)), more() {}

  friend bool operator==(list_1 const&, list_1 const&) = default;
};

template <typename T>
list_1(T, std::vector<T>) -> list_1<T>;

template <typename T>
list_1(T) -> list_1<T>;

// Variants and Expressions
struct expression_var {
  prefix_handle field;
  expr_handle property;

  friend bool operator==(expression_var const&,
                         expression_var const&) = default;
};

struct name_var {
  prefix_handle field;
  std::string property;

  friend bool operator==(name_var const&, name_var const&) = default;
};

using var = std::variant<std::string, expression_var, name_var>;

enum class field_sep {
  COMMA,
  SEMICOLON,
};

struct expr_field {
  expr_handle key;
  expr_handle value;

  friend bool operator==(expr_field const&, expr_field const&) = default;
};

struct name_field {
  std::string key;
  expr_handle value;

  friend bool operator==(name_field const&, name_field const&) = default;
};

struct value_field {
  expr_handle value;

  friend bool operator==(value_field const&, value_field const&) = default;
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

struct args {
  std::variant<std::vector<expr>, table, string> arguments;

  friend bool operator==(args const&, args const&) = default;
};

struct fn_call {
  prefix_handle prefix;
  std::optional<std::string> field_name;

  friend bool operator==(fn_call const&, fn_call const&) = default;
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

  friend bool operator==(binary_expr const&, binary_expr const&) = default;
};

struct unary_expr {
  unary_op op;
  expr_handle expr;

  friend bool operator==(unary_expr const&, unary_expr const&) = default;
};

struct prefix_expr {
  std::variant<var, fn_call, expr_handle> choices;

  friend bool operator==(prefix_expr const&, prefix_expr const&) = default;
};

struct expr {
  std::variant<nil, false_t, true_t, number, string, fn, prefix_expr, table,
               binary_expr, unary_expr, vararg>
      choices;

  friend bool operator==(expr const&, expr const&) = default;
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

struct fn_name {
  std::string name;
  std::vector<std::string> dot_names;
  std::optional<std::string> colon_name;

  friend bool operator==(fn_name const&, fn_name const&) = default;
};

struct fn_decl_stat {
  fn_name name;
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

struct statement {
  std::variant<var_assign_stat, do_stat, while_stat, repeat_stat, if_stat,
               for_range_stat, for_in_stat, fn_decl_stat, local_fn_decl_stat,
               var_decl_stat>
      choices;

  friend bool operator==(statement const&, statement const&) = default;
};

// Root of AST
using ast = block;

std::ostream& operator<<(std::ostream&, return_stat const&);
std::ostream& operator<<(std::ostream&, block const&);
std::ostream& operator<<(std::ostream&, unary_op const&);
std::ostream& operator<<(std::ostream&, binary_op const&);
std::ostream& operator<<(std::ostream&, expression_var const&);
std::ostream& operator<<(std::ostream&, name_var const&);
std::ostream& operator<<(std::ostream&, var const&);
std::ostream& operator<<(std::ostream&, field_sep const&);
std::ostream& operator<<(std::ostream&, expr_field const&);
std::ostream& operator<<(std::ostream&, name_field const&);
std::ostream& operator<<(std::ostream&, value_field const&);
std::ostream& operator<<(std::ostream&, field const&);
std::ostream& operator<<(std::ostream&, table const&);
std::ostream& operator<<(std::ostream&, string const&);
std::ostream& operator<<(std::ostream&, args const&);
std::ostream& operator<<(std::ostream&, fn_call const&);
std::ostream& operator<<(std::ostream&, nil const&);
std::ostream& operator<<(std::ostream&, false_t const&);
std::ostream& operator<<(std::ostream&, true_t const&);
std::ostream& operator<<(std::ostream&, number const&);
std::ostream& operator<<(std::ostream&, vararg const&);
std::ostream& operator<<(std::ostream&, fn_body const&);
std::ostream& operator<<(std::ostream&, fn const&);
std::ostream& operator<<(std::ostream&, binary_expr const&);
std::ostream& operator<<(std::ostream&, unary_expr const&);
std::ostream& operator<<(std::ostream&, prefix_expr const&);
std::ostream& operator<<(std::ostream&, expr const&);
std::ostream& operator<<(std::ostream&, var_assign_stat const&);
std::ostream& operator<<(std::ostream&, do_stat const&);
std::ostream& operator<<(std::ostream&, while_stat const&);
std::ostream& operator<<(std::ostream&, repeat_stat const&);
std::ostream& operator<<(std::ostream&, elseif_code const&);
std::ostream& operator<<(std::ostream&, else_code const&);
std::ostream& operator<<(std::ostream&, if_stat const&);
std::ostream& operator<<(std::ostream&, for_range_stat const&);
std::ostream& operator<<(std::ostream&, for_in_stat const&);
std::ostream& operator<<(std::ostream&, fn_name const&);
std::ostream& operator<<(std::ostream&, fn_decl_stat const&);
std::ostream& operator<<(std::ostream&, local_fn_decl_stat const&);
std::ostream& operator<<(std::ostream&, var_decl_stat const&);

template <typename T>
std::ostream& operator<<(std::ostream& os, list_1<T> const& lst) {
  os << "[ ";
  os << lst.first << " ";
  for (auto const& x : lst.more)
    os << x << " ";
  os << "]";
  return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, handle<T> const& lst) {
  if (lst == nullptr)
    os << "null";
  else
    os << *lst;
  return os;
}
}  // namespace lua::ast
