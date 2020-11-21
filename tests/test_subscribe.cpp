
#include <doctest/doctest.h>

#include <utility>

#include "cpprex/cpprex.h"
#include "null_reducer.h"

/* Test the state subscription feature.
 *
 * Use every kind of posible subscriber callbacks:
 *  - Pointer to member function.
 *  - Traditional function pointer.
 *  - Functor object.
 *  - Lambda function.
 */

/// Global flag used to test if the subscriber was notified of the state change.
int notifiedCount = 0;

/// Class that will be used as the state.
struct state_class_with_member_fuction {
  /// Member function to be used as a subscriber.
  void subscriber_member_function() const { notifiedCount++; }
};
// Use an alias for shorter signatures
using state = state_class_with_member_fuction;

/// Traditional function callback
void subscriber_normal_function(state) { notifiedCount++; }

/// Functor callback type
struct subscriber_functor {
  void operator()(state) { notifiedCount++; }
};

/// Lambda function callback
constexpr auto const subscriber_lambda = [](state) { notifiedCount++; };

TEST_CASE("Test subscription") {
  // Create the store
  auto s = rex::store{null_reducer<>{}, state_class_with_member_fuction{}};

  // Init the flag
  notifiedCount = 0;

  /// Test each one by separate.
  SUBCASE("Subscription in sequence") {
    // A closure for DRY testing
    auto const test_subcase = [&s](char const* subcase_name, auto&& subscriber) {
      SUBCASE(subcase_name) {
        s.subscribe(std::forward<decltype(subscriber)>(subscriber));
        s.dispatch(0);
        CHECK(notifiedCount == 1);
      }
    };

    // Test the subscription with a fuction pointer
    test_subcase("Function pointer suscriptor", &subscriber_normal_function);

    // Test the subscription with a functor
    test_subcase("Functor suscriptor", subscriber_functor{});

    // Test the subscription with a lambda
    test_subcase("Lambda function suscriptor", subscriber_lambda);

    // Test the subscription with a pointer to member function
    test_subcase("Member function suscriptor",
                 &state_class_with_member_fuction::subscriber_member_function);
  }

  /// Test each one in parallel by adding several subscriptors.
  SUBCASE("Subscription in parallel") {
    // Test the subscription with a fuction pointer
    s.subscribe(&subscriber_normal_function);

    // Test the subscription with a functor
    s.subscribe(subscriber_functor{});

    // Test the subscription with a lambda
    s.subscribe(subscriber_lambda);

    // Test the subscription with a pointer to member function
    s.subscribe(&state_class_with_member_fuction::subscriber_member_function);

    // Dispatch action
    s.dispatch(0);

    // It should have notified to 4 subscribers
    CHECK(notifiedCount == 4);
  }
}
