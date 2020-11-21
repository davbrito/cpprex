#include <doctest/doctest.h>

#include <vector>

#include "cpprex/cpprex.h"

/* Test with the tipical counter example.
 *
 * Make a store with an integer count.
 * The reducer manages 2 actions: `increment` and `decrement`
 */

namespace counter {

enum class action {
  increment,
  decrement,
};

inline auto reducer(int state, action action) noexcept -> int {
  switch (action) {
    case action::increment:
      return state + 1;
    case action::decrement:
      return state - 1;
  }
  // Unreachable. (gcc complains if no final return statement is found)
  return state;
}

}  // namespace counter

/// Dispatch a sequence of actions
constexpr auto dispatch_actions =
    [](auto& store, std::vector<counter::action> const& actions) {
      for (auto action : actions) {
        store.dispatch(action);
      }
    };

TEST_CASE("Counter test") {
  auto s = rex::store{&counter::reducer, 0};

  // Check initial state
  CHECK(s.get_state() == 0);

  SUBCASE("Mangages actions") {
    /// Increment and decrement actions shortcuts.
    auto const inc = counter::action::increment,
               dec = counter::action::decrement;

    dispatch_actions(s, {inc, inc, inc, inc, inc, inc, inc});
    CHECK(s.get_state() == 7);

    dispatch_actions(s, {dec, dec, dec, dec, dec, dec, dec, dec});
    CHECK(s.get_state() == -1);

    dispatch_actions(s, {inc, dec, inc, dec, dec, inc, inc, inc});
    CHECK(s.get_state() == 1);
  }
}
