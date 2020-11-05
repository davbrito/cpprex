#include <doctest/doctest.h>

#include <map>
#include <variant>

#include "cpprex/cpprex.h"

namespace counter {

enum class action { increment, decrement };

int reducer(int state, action action) {
  switch (action) {
    case action::increment:
      return state + 1;
    case action::decrement:
      return state - 1;
    default:
      return state;
  }
}

}  // namespace counter

TEST_CASE("Counter test") {
  rex::store s(counter::reducer, 0);

  CHECK_EQ(s.get_state(), 0);

  SUBCASE("Mangages actions") {
    SUBCASE("Increment") {
      s.dispatch(counter::action::increment);
      CHECK_EQ(s.get_state(), 1);
      SUBCASE("Increment again") {
        s.dispatch(counter::action::increment);
        CHECK_EQ(s.get_state(), 2);
      }
    }
    SUBCASE("Decrement") {
      s.dispatch(counter::action::decrement);
      CHECK_EQ(s.get_state(), -1);
      SUBCASE("Increment") {
        s.dispatch(counter::action::increment);
        CHECK_EQ(s.get_state(), 0);
      }
    }
  }
}

namespace user {
struct user {
  std::string first_name;
  std::string last_name;
  operator std::string() const {
    return "user { " + first_name + " " + last_name + " }";
  }
};

bool operator==(user const& l, user const& r) {
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

user reducer(user u, action a) {
  return std::visit(
      [&](auto&& a) -> user {
        using T = std::decay_t<decltype(a)>;
        if constexpr (std::is_same_v<T, actions::set_first_name>)
          return {a.fn, u.last_name};
        else if constexpr (std::is_same_v<T, actions::set_last_name>)
          return {u.first_name, a.ln};
      },
      a);
}

}  // namespace user

TEST_CASE("user test") {
  rex::store s(user::reducer, user::user{"david", "brito"});

  CHECK_EQ(s.get_state(), user::user{"david", "brito"});

  SUBCASE("Test actions") {
    SUBCASE("set_first_name") {
      s.dispatch(user::actions::set_first_name{"Juan"});
      CHECK_EQ(s.get_state(), user::user{"Juan", "brito"});
    }

    SUBCASE("set_last_name") {
      s.dispatch(user::actions::set_last_name{"Gonzalez"});
      CHECK_EQ(s.get_state(), user::user{"david", "Gonzalez"});
    }
  }
}