#include <doctest/doctest.h>

#include <string>
#include <variant>

#include "cpprex/cpprex.h"

/* Test with a store that has a struct as the state.
 *
 * The reducer manages 2 actions: `set_first_name` and `set_last_name`
 */

namespace user {
struct user {
  std::string first_name;
  std::string last_name;

  operator std::string() const {
    return "user { " + first_name + " " + last_name + " }";
  }

  inline friend bool operator==(user const &l, user const &r) {
    return l.first_name == r.first_name && l.last_name == r.last_name;
  }
};

namespace actions {

struct set_first_name {
  std::string first_name;
};
struct set_last_name {
  std::string last_name;
};

}  // namespace actions

using action_t = std::variant<actions::set_first_name, actions::set_last_name>;

inline user reducer(user usr, action_t const &action) {
  return std::visit(
      [&](auto &&act) -> user {
        using T = std::decay_t<decltype(act)>;
        if constexpr (std::is_same_v<T, actions::set_first_name>)
          return {act.first_name, usr.last_name};
        else if constexpr (std::is_same_v<T, actions::set_last_name>)
          return {usr.first_name, act.last_name};
      },
      action);
}

}  // namespace user

TEST_CASE("user test") {
  auto s = rex::store{user::reducer, user::user{"david", "brito"}};

  // Test initialization
  REQUIRE(s.get_state() == user::user{"david", "brito"});

  SUBCASE("Test actions") {
    SUBCASE("set_first_name") {
      s.dispatch(user::actions::set_first_name{"Juan"});
      CHECK(s.get_state() == user::user{"Juan", "brito"});
    }

    SUBCASE("set_last_name") {
      s.dispatch(user::actions::set_last_name{"Gonzalez"});
      CHECK(s.get_state() == user::user{"david", "Gonzalez"});
    }
  }
}
