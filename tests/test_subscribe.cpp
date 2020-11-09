
#include <doctest/doctest.h>

#include "cpprex/cpprex.h"
#include "null_reducer.h"

bool notified = false;

struct state_class_with_member_fuction {
  void subscriber_member_function() const { notified = true; }
};
using state = state_class_with_member_fuction;
void subscriber_normal_function(state) { notified = true; }

struct subscriber_functor {
  void operator()(state) { notified = true; }
};

constexpr auto subscriber_lambda = [](state) { notified = true; };

TEST_CASE("Test subscription") {
  rex::store s(null_reducer<>{}, state_class_with_member_fuction{});
  notified = false;

  SUBCASE("Function pointer suscriptor") {
    s.subscribe(subscriber_normal_function);
    s.dispatch(0);
    CHECK(notified);
  }

  SUBCASE("Functor suscriptor") {
    s.subscribe(subscriber_functor{});
    s.dispatch(0);
    CHECK(notified);
  }

  SUBCASE("Lambda function suscriptor") {
    s.subscribe(subscriber_lambda);
    s.dispatch(0);
    CHECK(notified);
  }

  SUBCASE("Member function suscriptor") {
    s.subscribe(&state_class_with_member_fuction::subscriber_member_function);
    s.dispatch(0);
    CHECK(notified);
  }
}
