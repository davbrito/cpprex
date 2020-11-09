#pragma once

#include <utility>

/// A reducer that returns the state unchanged. Ignoring any action.
template <typename T = void>
struct null_reducer {
  template <typename Action>
  T& operator()(T& x, Action&&) const {
    return x;
  }
  template <typename Action>
  T const& operator()(T const& x, Action&&) const {
    return x;
  }
};

/// A reducer that returns the state unchanged. Ignoring any action.
template <>
struct null_reducer<void> {
  template <typename T, typename Action>
  auto operator()(T&& x, Action &&) -> T {
    return std::forward<T>(x);
  }
};
