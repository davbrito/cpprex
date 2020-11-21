#include <doctest/doctest.h>

// Include <iostream> to prevent doctest compilation error is MSVC
#include <iostream>


#include "cpprex/cpprex.h"
#include "user.h"

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