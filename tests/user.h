#pragma once

#include <string>
#include <variant>

namespace user {
struct user {
  std::string first_name;
  std::string last_name;
  operator std::string() const {
    return "user { " + first_name + " " + last_name + " }";
  }
};

inline bool operator==(user const &l, user const &r) {
  return l.first_name == r.first_name && l.last_name == r.last_name;
}

namespace actions {

struct set_first_name {
  std::string fn;
};

struct set_last_name {
  std::string ln;
};

}  // namespace actions

using action = std::variant<actions::set_first_name, actions::set_last_name>;

inline user reducer(user u, action a) {
  return std::visit(
      [&](auto &&a) -> user {
        using T = std::decay_t<decltype(a)>;
        if constexpr (std::is_same_v<T, actions::set_first_name>)
          return {a.fn, u.last_name};
        else if constexpr (std::is_same_v<T, actions::set_last_name>)
          return {u.first_name, a.ln};
      },
      a);
}

}  // namespace user
