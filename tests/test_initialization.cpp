#include <doctest/doctest.h>

#include <string>
#include <vector>

#include "cpprex/cpprex.h"
#include "null_reducer.h"

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
       std::vector{"hola"s, "state"s, "Redux"s, ":)"s}) {
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
