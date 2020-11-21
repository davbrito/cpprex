#include <doctest/doctest.h>

// Include <iostream> to prevent compilation error is MSVC
#include <iostream>

#include "counter.h"
#include "cpprex/cpprex.h"
#include "user.h"

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