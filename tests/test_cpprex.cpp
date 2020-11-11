#include <doctest/doctest.h>

// Include <iostream> to prevent compilation error is MSVC
#include <iostream>
#include <string>
#include <vector>

#include "counter.h"
#include "cpprex/cpprex.h"
#include "null_reducer.h"
#include "user.h"

TEST_CASE("Initialization") {
  using namespace std::string_literals;

  for (int initial_state : {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}) {
    CAPTURE(initial_state);

    SUBCASE("Copy state") {
      rex::store s(null_reducer<>{}, initial_state);
      CHECK_EQ(s.get_state(), initial_state);
    }

    SUBCASE("Move state") {
      rex::store s(null_reducer<>{}, std::move(initial_state));
      CHECK_EQ(s.get_state(), initial_state);
    }
  }

  for (const auto& initial_state :
       std::vector<std::string>{"hola"s, "state"s, "Redux"s, ":)"s}) {
    CAPTURE(initial_state);

    SUBCASE("Copy state") {
      rex::store s(null_reducer<>{}, initial_state);
      CHECK_EQ(s.get_state(), initial_state);
    }

    SUBCASE("Move state") {
      rex::store s(null_reducer<>{}, std::move(initial_state));
      CHECK_EQ(s.get_state(), initial_state);
    }
  }
}

TEST_CASE("Counter test") {
  rex::store s(counter::reducer, 0);

  REQUIRE_EQ(s.get_state(), 0);

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

TEST_CASE("user test") {
  rex::store s(user::reducer, user::user{"david", "brito"});

  REQUIRE_EQ(s.get_state(), user::user{"david", "brito"});

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